#pragma once

#include <vector>

struct HierTreeNode;

class INodeRefinePredicate
{
  public:
    virtual ~INodeRefinePredicate() noexcept = default;
    virtual bool operator()(const HierTreeNode *htn, int dir) const = 0;
};
