#pragma once

#include "Defines.h"
#include "TopologyDefines.h"
#include <array>

namespace MeshGeneration
{
   using TriangleNodes = std::array<PointIndex, TopologyDefines::NumNodesOnTriangle>;
   using SortedEdge = std::array<PointIndex, TopologyDefines::NumNodesOnEdge>;
}