#pragma once

#include "INodeRefinePredicateFactory.h"

class NodeRefinePredicateFactoryByLevelOrSurplus : public INodeRefinePredicateFactory
{
  public:
    NodeRefinePredicateFactoryByLevelOrSurplus(int maxlevel, double minSurplus);
    std::unique_ptr<INodeRefinePredicate> create(const HierApproximation &approximation) override;

  private:
    int m_maxLevel;
    double m_minSurplus;
};
