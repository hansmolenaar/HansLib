#pragma once

#include <span>

namespace IntervalTree
{
   struct AdjacentDirection
   {
      int Direction;
      bool UsePositiveDirection;
   };

   std::span<AdjacentDirection> GetAdjacentNeighbors(int N);
}

