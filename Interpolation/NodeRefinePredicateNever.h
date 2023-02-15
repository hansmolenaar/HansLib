#pragma once

#include "INodeRefinePredicate.h"


class NodeRefinePredicateNever : public INodeRefinePredicate
{
public:
   bool operator()(const HierTreeNode* htn, int dir) const override { return false; }
};