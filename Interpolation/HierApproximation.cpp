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
      const std::function<bool(const HierRefinementInfo&)>& RefinementPredicate;
      double MaxSurplus = 0;

      bool operator()(const HierTreeNode* htn) const
      {
         if (!IsRefinable{ Factory }(htn)) return false;
         const auto& mli = htn->BasisFunction->getMultiIndex();
         const HierRefinementInfo refinementInfo{ HierMultiIndex(mli), std::abs(htn->Surplus), MaxSurplus };
         return RefinementPredicate(refinementInfo);
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


   void GetLeafNodesRecur(const std::vector<HierTreeNode*>& treeNodes, std::vector< HierTreeNode*>& result)
   {
      constexpr IsLeaf isLeaf;
      for (const auto& tn : treeNodes)
      {
         if (isLeaf(tn))
         {
            result.push_back(tn);
         }
         else
         {
            GetLeafNodesRecur(tn->Kids, result);
         }
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


#if false
   struct CreateKid
   {
      const IHierBasisFunction_Factory& Factory;
      const IMultiVariableRealValuedFunction& FunctionToApproximate;
      const HierApproximation& Approximation;

      std::unique_ptr<HierTreeNode> operator()(const HierMultiIndex& kidLevelIndex) const
      {
         auto kid = std::make_unique<HierTreeNode>(Factory.get(kidLevelIndex), 0.0);

         // Calculate the surplus
         const auto x = kidLevelIndex.toDoubles();
         const double functionEval = FunctionToApproximate.Evaluate(x);
         // Basis function may overlap
         const double approx = Approximation(x);
         kid->Surplus = functionEval - approx;
         return kid;
      }
   };
#endif

   HierTreeNode* GetOrCreate(const HierMultiIndex& hmi, const CreateHierNode& creator, std::map<HierMultiIndex, std::unique_ptr<HierTreeNode>>& treeNodeMap)
   {
      if (!treeNodeMap.contains(hmi))
      {
         auto ptr = creator(hmi);
         treeNodeMap.emplace(hmi, std::move(ptr));
      }

      return treeNodeMap.at(hmi).get();
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

std::unique_ptr<HierApproximation> HierApproximation::Create(const IMultiVariableRealValuedFunction& fie, const IHierBasisFunction_Factory& factory, const std::function<bool(const HierRefinementInfo&)>& doRefine)
{
   std::unique_ptr<HierApproximation> result(new HierApproximation(factory));

   DoRefine refinementPredicate{ factory, doRefine };


   const CreateHierNode createHierNode(factory, fie, *result);

   str::transform(factory.getLowestLevel(), std::back_inserter(result->m_root), [&createHierNode, &result](const auto& li) {return GetOrCreate(li, createHierNode, result->m_treeNodeMap); });

   std::vector<HierTreeNode*> toRefine;
   str::copy_if(result->getLeafNodes(), std::back_inserter(toRefine), refinementPredicate);

#if false
   while (!toRefine.empty())
   {
      for (auto ref : toRefine)
      {
         str::transform(ref->BasisFunction->getLevelIndex().refine(), std::back_inserter(ref->Kids), [&createKid, &result](const auto& li) {return GetOrCreate(li, createKid, result->m_treeNodeMap); });
      }

      toRefine.clear();
      refinementPredicate.MaxSurplus = result->getMaxSurplus();
      str::copy_if(result->getLeafNodes(), std::back_inserter(toRefine), refinementPredicate);
   }
#endif

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
   return result;
}

std::vector< HierTreeNode*> HierApproximation::getLeafNodes() const
{
   std::vector< HierTreeNode*> result;
   GetLeafNodesRecur(m_root, result);
   return result;
}