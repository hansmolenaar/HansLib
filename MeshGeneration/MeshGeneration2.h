#pragma once

#include "IndexTreeToSimplices2.h"
#include "MeshingStrategy.h"
#include "IGeometryRegion.h"

namespace MeshGeneration2
{
   IndexTreeToSimplices2::Triangles GenerateBaseTriangulation(const Geometry::IGeometryRegion<double,2>& region, MeshingStrategy2& strategy);
}