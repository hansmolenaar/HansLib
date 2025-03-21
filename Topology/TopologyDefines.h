#pragma once
#include "Defines.h"

#include <limits>

namespace Topology
{
   using TopologyDimension = unsigned int;
   inline constexpr TopologyDimension Corner = 0;
   inline constexpr TopologyDimension Edge = 1;
   inline constexpr TopologyDimension Face = 2;
   inline constexpr TopologyDimension Volume = 3;

   inline constexpr size_t NumNodesOnEdge = 2;
   inline constexpr size_t NumNodesOnTriangle = 3;

   using NodeIndex = PointIndex;
   inline constexpr NodeIndex NodeIndexInvalid = PointIndexInvalid;

   using DirectedEdgeNodes = std::array<NodeIndex, NumNodesOnEdge>;
   using SortedEdgeNodes = std::array<NodeIndex, NumNodesOnEdge>;

   using TriangleNodes = std::array<NodeIndex, NumNodesOnTriangle>;
}