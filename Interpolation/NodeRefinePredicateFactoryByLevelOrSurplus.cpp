#include "NodeRefinePredicateFactoryByLevelOrSurplus.h"
#include "HierApproximation.h"

class NodeRefinePredicateByLevelOrSurplus : public INodeRefinePredicate
{
  public:
    NodeRefinePredicateByLevelOrSurplus(int maxlevel, double minSurplus);
    bool operator()(const HierTreeNode *htn, int dir) const override;

  private:
    int m_maxLevel;
    double m_minSurplus;
};

NodeRefinePredicateByLevelOrSurplus::NodeRefinePredicateByLevelOrSurplus(int maxlevel, double minSurplus)
    : m_maxLevel(maxlevel), m_minSurplus(minSurplus)
{
}

bool NodeRefinePredicateByLevelOrSurplus::operator()(const HierTreeNode *htn, int dir) const
{
    if (htn->RefinementLevel < m_maxLevel)
        return true;
    if (std::abs(htn->Surplus) > m_minSurplus)
        return true;
    return false;
}

NodeRefinePredicateFactoryByLevelOrSurplus::NodeRefinePredicateFactoryByLevelOrSurplus(int maxlevel, double minSurplus)
    : m_maxLevel(maxlevel), m_minSurplus(minSurplus)
{
}

std::unique_ptr<INodeRefinePredicate> NodeRefinePredicateFactoryByLevelOrSurplus::create(
    const HierApproximation &approximation)
{
    return std::make_unique<NodeRefinePredicateByLevelOrSurplus>(m_maxLevel, m_minSurplus);
}