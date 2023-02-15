#include "NodeRefinePredicateFactoryByLevel.h"
#include "HierApproximation.h"
#include "Utilities/Defines.h"
#include "NodeRefinePredicateAlways.h"
#include "NodeRefinePredicateNever.h"

NodeRefinePredicateFactoryByLevel::NodeRefinePredicateFactoryByLevel(int maxlevel) :
   m_maxLevel(maxlevel)
{}

std::unique_ptr<INodeRefinePredicate> NodeRefinePredicateFactoryByLevel::create(const HierApproximation& approximation)
{
   const auto leaves = approximation.getLeafNodesRO();
   const auto maxLeaf = std::max_element(leaves.begin(), leaves.end(), [](const HierTreeNode* leaf1, const HierTreeNode* leaf2) {return leaf1->RefinementLevel < leaf2->RefinementLevel; });
   const int maxLevel = (*maxLeaf)->RefinementLevel;
   if (maxLevel < m_maxLevel) return std::make_unique<NodeRefinePredicateAlways>();
   return std::make_unique<NodeRefinePredicateNever>();
}