#pragma once

#include "IntervalTreeAdjacentDirection.h"

using namespace IntervalTree;

template<>
const std::array<AdjacentDirection, 2>& IntervalTree::GetAdjacentNeighbors<1>()
{
   static std::array<AdjacentDirection, 2> ngb1{ AdjacentDirection{0, false}, AdjacentDirection{0, true} };
   return ngb1;
}

template<>
const std::array<AdjacentDirection, 4>& IntervalTree::GetAdjacentNeighbors<2>()
{
   static std::array<AdjacentDirection, 4> ngb{
      AdjacentDirection{0, false}, AdjacentDirection{0, true},
      AdjacentDirection{1, false}, AdjacentDirection{1, true}
   };
   return ngb;
}

template<>
const std::array<AdjacentDirection, 6>& IntervalTree::GetAdjacentNeighbors<3>()
{
   static std::array<AdjacentDirection, 6> ngb{
      AdjacentDirection{0, false}, AdjacentDirection{0, true},
      AdjacentDirection{1, false}, AdjacentDirection{1, true},
      AdjacentDirection{2, false}, AdjacentDirection{2, true}
   };
   return ngb;
}

