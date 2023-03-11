#pragma once

#include "INodeRefinePredicate.h"
#include "INodeRefinePredicateFactory.h"


class NodeRefinePredicateNever : public INodeRefinePredicate
{
public:
   bool operator()(const HierTreeNode* htn, int dir) const override { return false; }
};





class NodeRefinePredicateFactoryNever : public INodeRefinePredicateFactory
{
public:
   NodeRefinePredicateFactoryNever() = default;
   std::unique_ptr<INodeRefinePredicate> create(const HierApproximation& approximation) override { return std::make_unique<NodeRefinePredicateNever>(); }
};
