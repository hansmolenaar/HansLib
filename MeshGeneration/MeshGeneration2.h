#pragma once

#include "IndexTreeToSimplices2.h"
#include "MeshingStrategy.h"
#include "IGeometryRegion.h"
#include "IDynamicUniquePointCollection.h"
#include "TrianglesNodes.h"
#include "IManifold1D2.h"
#include "Logger.h"
#include "VtkData.h"

namespace MeshGeneration2
{
   IndexTreeToSimplices2::Triangles GenerateBaseTriangulation(const Geometry::IGeometryRegion<double,2>& region, MeshingStrategy2& strategy, Logger& logger);

   void BaseTriangulationToWorld(
      const IndexTreeToSimplices2::Triangles& baseTriangles, 
      const IGeometryPredicate<double, 2>& predicate, 
      const BoundingBox<double,2>& worldBB, 
      std::unique_ptr<IDynamicUniquePointCollection<double,2>>& pointGeometry, 
      std::unique_ptr<MeshGeneration::TrianglesNodes>& trianglesNodes,
      Logger& logger);

   void InsertLineManifoldInTriangleByMovingPoints(const Geometry::IManifold1D2<double>& manifold, const MeshGeneration::TriangleNodes& triangleNodes, IDynamicUniquePointCollection<double, GeomDim2>& pointCollection);

   std::unique_ptr<Vtk::VtkData> ToVtkData(const MeshGeneration::TrianglesNodes& trianglesNodes, const IPointCollection<double,2>& points);
}