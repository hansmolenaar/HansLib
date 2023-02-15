#pragma once

#include "INodeRefinePredicateFactory.h"

class NodeRefinePredicateFactoryByLevel : public INodeRefinePredicateFactory
{
public:
  explicit NodeRefinePredicateFactoryByLevel(int maxlevel);
  std::unique_ptr<INodeRefinePredicate> create(const HierApproximation& approximation) override;
private:
   int m_maxLevel;
};
