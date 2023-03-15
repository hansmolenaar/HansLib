#include "NodeRefinePredicateAdaptive.h"
#include "HierApproximation.h"

#include <queue>
#include <iterator>

namespace
{
   using HTreeNodeValue = std::pair<const HierTreeNode*, double>;

   bool Compare(const HTreeNodeValue& a, const HTreeNodeValue& b)
   {
      return a.second < b.second;
   }



}

NodeRefinePredicateAdaptive::NodeRefinePredicateAdaptive(std::set<const HierTreeNode*> toRefine) :
   m_toRefine(std::move(toRefine))
{
}

bool NodeRefinePredicateAdaptive::operator()(const HierTreeNode* htn, int dir) const
{
   return m_toRefine.find(htn) != m_toRefine.end();
}

NodeRefinePredicateFactoryAdaptive::NodeRefinePredicateFactoryAdaptive(double alpha, double fraction) :
   m_alpha(alpha),
   m_fraction(fraction)
{}



std::unique_ptr<INodeRefinePredicate> NodeRefinePredicateFactoryAdaptive::create(const HierApproximation& approximation)
{
   const auto leafNodes = approximation.getLeafNodesRO();
   const size_t siz = leafNodes.size();
   std::vector<HTreeNodeValue> withRang(siz);
   str::transform(leafNodes, withRang.begin(), [&approximation](const HierTreeNode* htn) {return std::make_pair(htn, approximation(htn->getMultiIndex().toDoubles())); });
   str::sort(withRang, Compare);

   std::map<const HierTreeNode*, double> nodeToValue;
   const auto factorRang = (1 - m_alpha) / (1.0 * siz);
   for (size_t n = 0; n < siz; ++n)
   {
      nodeToValue[withRang.at(n).first] = n * factorRang;
   }

   // Avoid 0/0
   const auto factorLevel = m_alpha / (approximation.getMaxLevel() + 1.0);
   for (const auto* leafNode : leafNodes)
   {
      nodeToValue[leafNode] += factorLevel * leafNode->getLevel();
   }

   std::vector<HTreeNodeValue> priority(siz);
   str::transform(nodeToValue, priority.begin(), [](const auto& itr) {return std::make_pair(itr.first, itr.second); });
   str::sort(priority, Compare);

   const auto numToSelect =std::min(siz, std::max(size_t(2), size_t(std::round(m_fraction * siz))));
   std::set<const HierTreeNode*> toRefine;
   for (size_t n = 0; n < numToSelect; ++n)
   {
      toRefine.insert(priority.at(n).first);
   }
   // Also add almost equal values to avoid instabilities
   for (size_t n = numToSelect; n < priority.size(); ++n)
   {
      if (std::abs(priority.at(n).second - priority.at(n - 1).second) > 1.0e-10) break;
      toRefine.insert(priority.at(n).first);
   }

   return std::make_unique<NodeRefinePredicateAdaptive>(toRefine);
}