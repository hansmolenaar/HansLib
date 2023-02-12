#include "HierApproximation.h"
#include "Utilities/Defines.h"
#include "Utilities/Functors.h"
#include "Utilities/Logger.h"

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
         const auto& mli = htn->getMultiIndex();
         return Factory.canBeRefined(mli);
      }
   };


   struct CreateHierNode
   {
      const IHierBasisFunction_Factory& Factory;
      const IMultiVariableRealValuedFunction& FunctionToApproximate;
      const HierApproximation& Approximation;

      std::unique_ptr<HierTreeNode> operator()(const HierMultiIndex& hmi, int level) const
      {
         const auto* basisfunction = Factory.get(hmi);
         auto node = std::make_unique<HierTreeNode>(basisfunction);

         // Calculate the surplus
         const auto& xyz = hmi.toDoubles();
         node->Value = FunctionToApproximate.Evaluate(xyz);
         // Basis function may overlap
         const double approx = Approximation(xyz);
         node->Surplus = node->Value - approx;
         node->RefinementLevel = level;
         return node;
      }
   };

   HierTreeNode* Get(const HierMultiIndex& hmi, std::map<HierMultiIndex, std::unique_ptr<HierTreeNode>>& treeNodeMap)
   {
      const auto& found = treeNodeMap.find(hmi);
      if (found == treeNodeMap.end()) throw MyException("Get() not found: " + hmi.toString());
      return found->second.get();
   }

   // Return nullptr if already existing
   HierTreeNode* CreateIfNew(const HierMultiIndex& hmi, int level, const CreateHierNode& creator, std::map<HierMultiIndex, std::unique_ptr<HierTreeNode>>& treeNodeMap)
   {
      if (treeNodeMap.contains(hmi))
      {
         if (treeNodeMap.at(hmi)->RefinementLevel != level) throw MyException("CreateIfNew");
         return nullptr;
      }
      treeNodeMap.emplace(hmi, creator(hmi, level));
      return Get(hmi, treeNodeMap);
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

std::unique_ptr<HierApproximation> HierApproximation::Create(const IMultiVariableRealValuedFunction& fie, const IHierBasisFunction_Factory& factory, const GetRefinements& getRefinements)
{
   Logger logger;
   std::vector<std::string> loglines;
   std::unique_ptr<HierApproximation> result(new HierApproximation(factory));

   const CreateHierNode createHierNode(factory, fie, *result);

   for (const auto& hmi : factory.getLowestLevel())
   {
      auto* kid = CreateIfNew(hmi, 0, createHierNode, result->m_treeNodeMap);
      if (kid != nullptr) result->m_root.push_back(kid);
   }

   for (const auto& tr : result->m_treeNodeMap) loglines.push_back(tr.first.toString());
   logger.LogLine(loglines);

   auto toRefine = getRefinements(*result);

   while (!toRefine.empty())
   {
#if false
      loglines = std::vector<std::string> { {"cells to refine:"} };
      for (const auto& tr : toRefine) loglines.push_back(tr.first.toString() + " -> " + std::to_string(tr.second));
      logger.LogLine(loglines);
#endif

      for (auto& parentDir : toRefine)
      {
         for (const auto& kid : parentDir.first.refine(parentDir.second))
         {
            auto* parent = Get(parentDir.first, result->m_treeNodeMap);
            auto* kidPtr = CreateIfNew(kid, parent->RefinementLevel + 1, createHierNode, result->m_treeNodeMap);
            if (kidPtr != nullptr) parent->Kids.emplace_back(kidPtr);
         }
      }

#if false
      loglines = std::vector<std::string>{ {"after refinement:"} };
      for (const auto& tr : result->m_treeNodeMap) loglines.push_back(tr.first.toString());
      loglines.push_back("Total number of cells: " + std::to_string(result->getAllTreeNodesRO().size()));
      loglines = std::vector<std::string>{ {"leaf nodes:"} };
      for (const auto& leaf : result->getLeafNodesRO()) loglines.push_back(leaf->BasisFunction->getMultiIndex().toString());
      logger.LogLine(loglines);
#endif
  
      toRefine = getRefinements(*result);
   }


#if true
   loglines = std::vector<std::string>{ {"READY"} };
   loglines.push_back("Number of cells: " + std::to_string(result->getAllTreeNodesRO().size()));
   loglines.push_back("Number of leaf cells: " + std::to_string(result->getLeafNodesRO().size()));
   logger.LogLine(loglines);
#endif

   return result;
}

double HierApproximation::operator()(std::span<const double> xyz) const
{
   double result = 0;
   for (const auto* node : m_root)
   {
      result += (*node)(xyz);
   }
   return result;
}

std::vector< HierTreeNode*> HierApproximation::getAllTreeNodes() const
{
   std::vector< HierTreeNode*> result(m_treeNodeMap.size());
   str::transform(m_treeNodeMap, result.begin(), [](const auto& itr) {return itr.second.get(); });
   return result;
}

std::vector<const HierTreeNode*> HierApproximation::getAllTreeNodesRO() const
{
   std::vector<const HierTreeNode*> result(m_treeNodeMap.size());
   str::transform(m_treeNodeMap, result.begin(), [](const auto& itr) {return itr.second.get(); });
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
   std::vector<const HierTreeNode*> result;
   str::copy_if(getAllTreeNodesRO(), std::back_inserter(result), IsLeaf());
   return result;
}

double HierApproximation::getMaxSurplus() const
{
   std::vector< HierTreeNode*> active;
   str::copy_if(getLeafNodes(), std::back_inserter(active), IsRefinable{ m_factory });
   return str::max(active | stv::transform([](const HierTreeNode* leaf) {return std::abs(leaf->Surplus); }));
}

std::vector<std::vector<double>> HierApproximation::getCollocationPoints() const
{
   const std::vector<const HierTreeNode*> allTreeNodes = getAllTreeNodesRO();
   std::vector<std::vector<double>> result(allTreeNodes.size());
   str::transform(allTreeNodes, result.begin(), [](const HierTreeNode* tn) {return std::vector<double>{ tn->getMultiIndex().toDoubles()}; });
   str::sort(result);
   return result;
}

const IHierBasisFunction_Factory& HierApproximation::getFactory() const
{
   return m_factory;
}