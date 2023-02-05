#include "HierApproximation.h"
#include "Utilities/Defines.h"

namespace
{
   struct IsLeaf
   {
      bool operator()(const HierTreeNode* htn) const
      {
         return htn->Kids.empty();
      }
   };


   struct IsRefinable
   {
      const IHierBasisFunction_Factory& Factory;

      bool operator()(const HierTreeNode* htn) const
      {
         const auto& mli = htn->BasisFunction->getMultiIndex();
         return Factory.canBeRefined(mli);
      }
   };


   struct DoRefine
   {
      const IHierBasisFunction_Factory& Factory;
      const HierApproximation::RefineInDirections& RefinementTest;
      double MaxSurplus = 0;

      std::vector<size_t> operator()(const HierTreeNode* htn) const
      {
         if (!IsRefinable{ Factory }(htn)) return {};
         const auto& mli = htn->BasisFunction->getMultiIndex();
         const HierRefinementInfo refinementInfo{ HierMultiIndex(mli), std::abs(htn->Surplus), MaxSurplus };
         return RefinementTest(refinementInfo);
      }
   };

   void GetAllTreeNodesRecur(const std::vector<HierTreeNode*>& treeNodes, std::vector< HierTreeNode*>& result)
   {
      str::copy(treeNodes, std::back_inserter(result));
      for (const auto& tn : treeNodes)
      {
         GetAllTreeNodesRecur(tn->Kids, result);
      }
   }

   struct CreateHierNode
   {
      const IHierBasisFunction_Factory& Factory;
      const IMultiVariableRealValuedFunction& FunctionToApproximate;
      const HierApproximation& Approximation;

      std::unique_ptr<HierTreeNode> operator()(const HierMultiIndex& hmi) const
      {
         const auto* basisfunction = Factory.get(hmi);
         auto kid = std::make_unique<HierTreeNode>(basisfunction);

         // Calculate the surplus
         const auto& xyz = hmi.toDoubles();
         const double functionEval = FunctionToApproximate.Evaluate(xyz);
         // Basis function may overlap
         const double approx = Approximation(xyz);
         kid->Surplus = functionEval - approx;
         return kid;
      }
   };


   HierTreeNode* GetOrCreate(const HierMultiIndex& hmi, const CreateHierNode& creator, std::map<HierMultiIndex, std::unique_ptr<HierTreeNode>>& treeNodeMap)
   {
      if (!treeNodeMap.contains(hmi))
      {
         auto ptr = creator(hmi);
         treeNodeMap.emplace(hmi, std::move(ptr));
      }

      return treeNodeMap.at(hmi).get();
   }

   std::vector<std::pair<HierTreeNode*, size_t>> GetRefinements(const HierApproximation& approximation, const DoRefine& doRefine)
   {
      std::vector<std::pair<HierTreeNode*, size_t>> result;
      for (const auto& leaf : approximation.getLeafNodes())
      {
         for (size_t dir : doRefine(leaf))
         {
            result.emplace_back(leaf, dir);
         }
      }
      return result;
   }

}

HierApproximation::HierApproximation(const IHierBasisFunction_Factory& factory) :
   m_factory(factory),
   m_indexer(MultiIndex<size_t>::Create(std::vector<size_t>{2, factory.getDimension()}))
{
}


double HierTreeNode::operator()(std::span<const double> x) const
{
   if (!BasisFunction->contains(x)) return 0;
   double result = Surplus * (*BasisFunction)(x);;
   for (const auto& kid : Kids)
   {
      result += (*kid)(x);
   }
   return result;
}

std::unique_ptr<HierApproximation> HierApproximation::Create(const IMultiVariableRealValuedFunction& fie, const IHierBasisFunction_Factory& factory, const RefineInDirections& refineInDirections)
{
   std::unique_ptr<HierApproximation> result(new HierApproximation(factory));
   DoRefine refinementPredicate{ factory, refineInDirections };

   const CreateHierNode createHierNode(factory, fie, *result);

   str::transform(factory.getLowestLevel(), std::back_inserter(result->m_root), [&createHierNode, &result](const auto& li) {return GetOrCreate(li, createHierNode, result->m_treeNodeMap); });

   std::vector<std::pair<HierTreeNode*, size_t>> toRefine = GetRefinements(*result, refinementPredicate);

   while (!toRefine.empty())
   {
      for (auto& ref : toRefine)
      {
         for (const auto& kid : ref.first->BasisFunction->getMultiIndex().refine(ref.second))
         {
            ref.first->Kids.emplace_back(GetOrCreate(kid, createHierNode, result->m_treeNodeMap));
         }
      }

      toRefine.clear();
      //refinementPredicate.MaxSurplus = result->getMaxSurplus();
      toRefine = GetRefinements(*result, refinementPredicate);
   }

   return result;
}


double HierApproximation::operator()(std::span<const double> xyz) const
{
   double result = 0;
   for (const auto& r : m_root)
   {
      result += (*r)(xyz);
   }
   return result;
}

std::vector< HierTreeNode*> HierApproximation::getAllTreeNodes() const
{
   std::vector< HierTreeNode*> result;
   GetAllTreeNodesRecur(m_root, result);
   // Remove the duplicates
   str::sort(result);
   result.erase(std::unique(result.begin(), result.end()), result.end());
   if (result.size() != m_treeNodeMap.size()) throw MyException("HierApproximation::getAllTreeNodes problem");
   return result;
}

std::vector<const HierTreeNode*> HierApproximation::getAllTreeNodesRO() const
{
   const auto& treeNodes = getAllTreeNodes();
   std::vector<const HierTreeNode*> result(treeNodes.size());
   str::transform(treeNodes, result.begin(), [](auto* ptr) {return ptr; });
   return result;
}

std::vector< HierTreeNode*> HierApproximation::getLeafNodes() const
{
   std::vector< HierTreeNode*> result;
   str::copy_if(getAllTreeNodes(), std::back_inserter(result), IsLeaf());
   return result;
}

std::vector<const HierTreeNode*> HierApproximation::getLeafNodesRO() const
{
   const auto& leafNodes = getLeafNodes();
   std::vector<const HierTreeNode*> result(leafNodes.size());
   str::transform(leafNodes, result.begin(), [](auto* ptr) {return ptr; });
   return result;
}

double HierApproximation::getMaxSurplus() const
{
   std::vector< HierTreeNode*> active;
   str::copy_if(getLeafNodes(), std::back_inserter(active), IsRefinable{ m_factory });
   return str::max(active | stv::transform([](const HierTreeNode* leaf) {return std::abs(leaf->Surplus); }));
}