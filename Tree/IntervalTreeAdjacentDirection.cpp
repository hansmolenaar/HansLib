#pragma once

#include "IntervalTreeAdjacentDirection.h"

using namespace IntervalTree;

const std::array<AdjacentDirection, 2>& IntervalTree::GetAdjacentNeighbors1()
{
   static const std::array<AdjacentDirection, 2> result{
     AdjacentDirection{0, false}, AdjacentDirection{0, true} };
   return result;
}

const std::array<AdjacentDirection, 4>& IntervalTree::GetAdjacentNeighbors2()
{
   static const std::array<AdjacentDirection, 4> result{
     AdjacentDirection{0, false}, AdjacentDirection{0, true},
        AdjacentDirection{ 1, false }, AdjacentDirection{ 1, true } };
   return result;
}

const std::array<AdjacentDirection, 6>& IntervalTree::GetAdjacentNeighbors3()
{
   static const std::array<AdjacentDirection, 6> result{
      AdjacentDirection{0, false}, AdjacentDirection{0, true},
      AdjacentDirection{1, false}, AdjacentDirection{1, true},
      AdjacentDirection{2, false}, AdjacentDirection{2, true}
   };
   return result;
}

template<>
const std::array<AdjacentDirection, 2>& IntervalTree::GetAdjacentNeighbors<1>()
{
   return GetAdjacentNeighbors1();
}

template<>
const std::array<AdjacentDirection, 4>& IntervalTree::GetAdjacentNeighbors<2>()
{
   return GetAdjacentNeighbors2();
}

template<>
const std::array<AdjacentDirection, 6>& IntervalTree::GetAdjacentNeighbors<3>()
{
   return GetAdjacentNeighbors3();
}

