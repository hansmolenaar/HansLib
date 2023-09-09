#pragma once

#include "IntervalTreeAdjacentDirection.h"
#include "IntervalTreeDefines.h"
#include "MyException.h"
#include <array>

using namespace IntervalTree;


std::span<AdjacentDirection> IntervalTree::GetAdjacentNeighbors(int N)
{
   static std::array<AdjacentDirection, 2> ngb1{ AdjacentDirection{0, false}, AdjacentDirection{0, true} };
   if (N == 1) return ngb1;
   throw MyException("IntervalTree::GetAdjacentNeighbors not implemented for N=" + std::to_string(N));
}

