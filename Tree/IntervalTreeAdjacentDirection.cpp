#pragma once

#include "IntervalTreeAdjacentDirection.h"

using namespace IntervalTree;

constexpr std::array<AdjacentDirection, 4> IntervalTree::GetAdjacentNeighbors2()
{
   constexpr std::array<AdjacentDirection, 4> result{
     AdjacentDirection{0, false}, AdjacentDirection{0, true},
        AdjacentDirection{ 1, false }, AdjacentDirection{ 1, true } };
   return result;
}

constexpr std::array<AdjacentDirection, 2> IntervalTree::GetAdjacentNeighbors1()
{
   constexpr std::array<AdjacentDirection, 2> result{
     AdjacentDirection{0, false}, AdjacentDirection{0, true} };
   return result;
}


constexpr std::array<AdjacentDirection, 6> IntervalTree::GetAdjacentNeighbors3()
{
   constexpr std::array<AdjacentDirection, 6> result{
      AdjacentDirection{0, false}, AdjacentDirection{0, true},
      AdjacentDirection{1, false}, AdjacentDirection{1, true},
      AdjacentDirection{2, false}, AdjacentDirection{2, true}
   };
   return result;
}

template<>
const std::array<AdjacentDirection, 2>& IntervalTree::GetAdjacentNeighbors<1>()
{
   static std::array<AdjacentDirection, 2> ngb = GetAdjacentNeighbors1();
   return ngb;
}

template<>
const std::array<AdjacentDirection, 4>& IntervalTree::GetAdjacentNeighbors<2>()
{
   static std::array<AdjacentDirection, 4> ngb = GetAdjacentNeighbors2();
   return ngb;
}

template<>
const std::array<AdjacentDirection, 6>& IntervalTree::GetAdjacentNeighbors<3>()
{
   static std::array<AdjacentDirection, 6> ngb = GetAdjacentNeighbors3();
   return ngb;
}

