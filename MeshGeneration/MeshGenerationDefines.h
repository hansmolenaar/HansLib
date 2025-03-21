#pragma once

#include "Defines.h"
#include "IDynamicUniquePointCollection.h"
#include "Simplex.h"
#include "TopologyDefines.h"

#include <boost/container/static_vector.hpp>
#include <functional>

namespace MeshGeneration
{
   using GeomType = double;

   using TriangleNodes = std::array<Topology::NodeIndex, Topology::NumNodesOnTriangle>;

   using IUniquePointCollection2 = IDynamicUniquePointCollection<MeshGeneration::GeomType, GeomDim2>;

   // Should be GE 0 and 0 means degenerate
   template<int N>
   using CellQualityFun = double(const Geometry::Simplex<GeomType, N>&);

   using CellQuality2Fun = double(const Geometry::Simplex<GeomType, GeomDim2>&);
}