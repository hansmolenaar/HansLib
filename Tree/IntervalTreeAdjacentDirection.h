#pragma once
#include <array>

namespace IntervalTree
{
   struct AdjacentDirection
   {
      int Direction;
      bool UsePositiveDirection;
      auto operator<=>(const AdjacentDirection&) const = default;
   };

   template<int N>
   const std::array<AdjacentDirection, 2 * N>& GetAdjacentNeighbors();

   template<>
   const std::array<AdjacentDirection, 2>& GetAdjacentNeighbors<1>();

   template<>
   const std::array<AdjacentDirection, 4>& GetAdjacentNeighbors<2>();

   template<>
   const std::array<AdjacentDirection, 6>& GetAdjacentNeighbors<3>();
}

