#pragma once

#include "IndexTreeToSimplices2.h"
#include "MeshingStrategy.h"
#include "IGeometryRegion.h"
#include "IDynamicUniquePointCollection.h"
#include "TriangleNodes.h"

namespace MeshGeneration2
{
   IndexTreeToSimplices2::Triangles GenerateBaseTriangulation(const Geometry::IGeometryRegion<double,2>& region, MeshingStrategy2& strategy);

   void BaseTriangulationToWorld(
      const IndexTreeToSimplices2::Triangles& baseTriangles, 
      const IGeometryPredicate<double, 2>& predicate, 
      const BoundingBox<double,2>& worldBB, 
      std::unique_ptr<IDynamicUniquePointCollection<double,2>>& pointGeometry, 
      std::unique_ptr<MeshGeneration::TriangleNodes>& triangleNodes );
}