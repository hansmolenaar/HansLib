#pragma once

#include "Defines.h"
#include "TopologyDefines.h"
#include <boost/container/static_vector.hpp>
#include <array>

namespace MeshGeneration
{
   using GeomType = double;
   using NodeIndex = PointIndex;
   using TriangleNodes = std::array<PointIndex, Topology::NumNodesOnTriangle>;
   using SortedEdge = std::array<PointIndex, Topology::NumNodesOnEdge>;
}