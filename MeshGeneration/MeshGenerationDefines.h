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

   // Should be GE 0 and 0 means degenerate
   template<int N>
   using CellQualityFun = double(const Geometry::Simplex<GeomType, N>&);

   using CellQuality2Fun = double(const Geometry::Simplex<GeomType, GeomDim2>&);
}

