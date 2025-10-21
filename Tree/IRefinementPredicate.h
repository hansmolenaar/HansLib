#pragma once

#include "IntervalTree.h"

template <size_t N> class IRefinementPredicate
{
  public:
    ~IRefinementPredicate() noexcept = default;
    virtual bool operator()(const IntervalTree::Index<N> &indx) const = 0;
};
