#pragma once

#include "INodeRefinePredicate.h"

class NodeRefinePredicateAlways : public INodeRefinePredicate
{
  public:
    bool operator()(const HierTreeNode *htn, int dir) const override
    {
        return true;
    }
};

class NodeRefinePredicateFactoryAlways : public INodeRefinePredicateFactory
{
  public:
    NodeRefinePredicateFactoryAlways() = default;
    std::unique_ptr<INodeRefinePredicate> create(const HierApproximation &approximation) override
    {
        return std::make_unique<NodeRefinePredicateAlways>();
    }
};