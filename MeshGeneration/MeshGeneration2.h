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
   IndexTreeToSimplices2::Triangles GenerateBaseTriangulation(const Geometry::IGeometryRegion<MeshGeneration::GeomType,GeomDim2>& region, MeshingStrategy2& strategy, Logger& logger);

   void BaseTriangulationToWorld(
      const IndexTreeToSimplices2::Triangles& baseTriangles, 
      const IGeometryPredicate<MeshGeneration::GeomType, GeomDim2>& predicate,
      const BoundingBox<MeshGeneration::GeomType,GeomDim2>& worldBB,
      std::unique_ptr<IDynamicUniquePointCollection<MeshGeneration::GeomType,GeomDim2>>& pointGeometry,
      std::unique_ptr<MeshGeneration::TrianglesNodes>& trianglesNodes,
      Logger& logger);

   void InsertLineManifoldInTriangleByMovingPoints(const Geometry::IManifold1D2<MeshGeneration::GeomType>& manifold, const MeshGeneration::TriangleNodes& triangleNodes, IDynamicUniquePointCollection<MeshGeneration::GeomType, GeomDim2>& pointCollection);

   std::unique_ptr<Vtk::VtkData> ToVtkData(const MeshGeneration::TrianglesNodes& trianglesNodes, const IPointCollection<MeshGeneration::GeomType,GeomDim2>& points);
}