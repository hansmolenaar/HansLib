#pragma once

#include "IntervalTreeAdjacentDirection.h"

using namespace IntervalTree;

template<>
const std::array<AdjacentDirection, 2>& IntervalTree::GetAdjacentNeighbors<1>()
{
   static std::array<AdjacentDirection, 2> ngb1{ AdjacentDirection{0, false}, AdjacentDirection{0, true} };
   return ngb1;
}

