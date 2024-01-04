#pragma once

#include "IntervalTree.h"

template<int N>
class IRefinementPredicate
{
public:
   ~IRefinementPredicate() noexcept = default;
   virtual bool operator()(const IntervalTree::Index<N>& indx) const = 0;
};