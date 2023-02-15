#pragma once

#include "INodeRefinePredicate.h"


class NodeRefinePredicateAlways : public INodeRefinePredicate
{
public:
   bool operator()(const HierTreeNode* htn, int dir) const override { return true; }
};