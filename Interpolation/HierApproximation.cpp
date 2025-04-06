#include "Defines.h"
#include "Functors.h"
#include "HierApproximation.h"
#include "Logger.h"

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
      IHierBasisFunction_Factory& Factory;
      const IMultiVariableFunctionEvaluate& FunctionToApproximate;
      const HierApproximation& Approximation;

      std::unique_ptr<HierTreeNode> operator()(const HierMultiIndex& hmi, int level) const
      {
         const auto* basisfunction = Factory.get(hmi);
         auto node = std::make_unique<HierTreeNode>(basisfunction);

         // Calculate the surplus
         const auto& xyz = hmi.toDoubles();
         node->Value = FunctionToApproximate(xyz);
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
      if (found == treeNodeMap.end())
      {
         std::ostringstream os;
         os << "Get() not found: " << hmi;
         throw MyException(os.str());
      }
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

   std::vector<HierApproximation::RefinementSpecification> GetAllRefinements(const HierApproximation& approx, const IHierBasisFunction_Factory& basisFunctionFactory, INodeRefinePredicateFactory& refinementFactory)
   {
      std::vector<HierApproximation::RefinementSpecification> result;
      const auto predicate = refinementFactory.create(approx);
      const int numDirections = static_cast<int>(basisFunctionFactory.getDimension());
      for (const auto* leaf : approx.getLeafNodesRO())
      {
         const auto& mi = leaf->getMultiIndex();
         if (!basisFunctionFactory.canBeRefined(mi)) continue;
         for (int d = 0; d < numDirections; ++d)
         {
            if ((*predicate)(leaf, d))
            {
               result.emplace_back(mi, d);
            }
         }
      }
      return result;
   }


   bool CompareLevel(const HierTreeNode* a, const HierTreeNode* b)
   {
      return a->getLevel() < b->getLevel();
   }


}

HierApproximation::HierApproximation(IHierBasisFunction_Factory& factory, const IMultiVariableFunctionEvaluate& function) :
   m_function(function),
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

std::unique_ptr<HierApproximation> HierApproximation::Create(const IMultiVariableFunctionEvaluate& fie, IHierBasisFunction_Factory& factory, INodeRefinePredicateFactory& refinementFactory)
{
   std::ostringstream os;
   Logger logger;
   std::vector<std::string> loglines;
   std::unique_ptr<HierApproximation> result(new HierApproximation(factory, fie));

   const CreateHierNode createHierNode(factory, fie, *result);

   for (const auto& hmi : factory.getLowestLevel())
   {
      auto* kid = CreateIfNew(hmi, 0, createHierNode, result->m_treeNodeMap);
      if (kid != nullptr) result->m_root.push_back(kid);
   }

   for (const auto& tr : result->m_treeNodeMap)
   {
      os << tr.first << "\n";
   }

   while (!result->iterate(refinementFactory).empty())
   {
      loglines = std::vector<std::string>{ {"after refinement:"} };
      for (const auto& tr : result->m_treeNodeMap)
      {
         os << tr.first << "\n";
      }
      os << "Total number of cells: " << result->getAllTreeNodesRO().size() << "\n";
      os << "leaf nodes:" << "\n";
      for (const auto& leaf : result->getLeafNodesRO())
      {
         os << leaf->BasisFunction->getMultiIndex() << "\n";
      }

   }


#if true
   os << "READY\n";
   os << "Number of cells: " << result->getAllTreeNodesRO().size() << "\n";
   os << "Number of leaf cells: " << result->getLeafNodesRO().size() << "\n";
#endif

   logger.logLine(os);
   return result;
}

// Return just creted nodes
std::vector<const HierTreeNode*> HierApproximation::iterate(INodeRefinePredicateFactory& refinementFactory)
{
   std::vector<const HierTreeNode*> result;
   const CreateHierNode createHierNode(m_factory, m_function, *this);
   auto toRefine = GetAllRefinements(*this, m_factory, refinementFactory);
   for (auto& parentDir : toRefine)
   {
      for (const auto& kid : parentDir.first.refine(parentDir.second))
      {
         auto* parent = Get(parentDir.first, m_treeNodeMap);
         auto* kidPtr = CreateIfNew(kid, parent->RefinementLevel + 1, createHierNode, m_treeNodeMap);
         if (kidPtr != nullptr)
         {
            parent->Kids.emplace_back(kidPtr);
            result.push_back(kidPtr);
         }
      }
   }
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


int HierApproximation::getMaxLevel() const
{
   const auto treeNodes = getAllTreeNodesRO();
   return (*str::max_element(treeNodes, CompareLevel))->RefinementLevel;
}