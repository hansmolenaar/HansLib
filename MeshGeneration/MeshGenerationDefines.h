#pragma once

#include "Defines.h"
#include "TopologyDefines.h"
#include "IDynamicUniquePointCollection.h"

#include <boost/container/static_vector.hpp>
#include <array>

namespace MeshGeneration
{
   using GeomType = double;
   using NodeIndex = PointIndex;
   using TriangleNodes = std::array<NodeIndex, Topology::NumNodesOnTriangle>;
   using DirectedEdgeNodes = std::array<NodeIndex, Topology::NumNodesOnEdge>;
   using SortedEdgeNodes = std::array<NodeIndex, Topology::NumNodesOnEdge>;

   using IUniquePointCollecion2 = IDynamicUniquePointCollection<MeshGeneration::GeomType, GeomDim2>;
}