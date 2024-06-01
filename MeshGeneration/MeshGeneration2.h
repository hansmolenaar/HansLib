#pragma once

#include "IndexTreeToSimplices2.h"
#include "IGeometryRegion.h"
#include "IDynamicUniquePointCollection.h"
#include "MeshGenerationDefines.h"
#include "TrianglesNodes.h"
#include "IManifold1D2.h"
#include "IManifold0.h"
#include "ManifoldsAndNodes.h"
#include "Logger.h"
#include "VtkData.h"
#include "IMeshingSettings.h"
#include "IManifold0.h"

namespace MeshGeneration2
{
   using IManifold0D2 = Geometry::IManifold0<MeshGeneration::GeomType, GeomDim2>;

   IndexTreeToSimplices2::Triangles GenerateBaseTriangulation(const Geometry::IGeometryRegion<MeshGeneration::GeomType, GeomDim2>& region, MeshGeneration::IMeshingSettings2& settings);

   void BaseTriangulationToWorld(
      const IndexTreeToSimplices2::Triangles& baseTriangles,
      const IGeometryPredicate<MeshGeneration::GeomType, GeomDim2>& predicate,
      const BoundingBox<MeshGeneration::GeomType, GeomDim2>& worldBB,
      std::unique_ptr<MeshGeneration::IUniquePointCollecion2>& pointGeometry,
      std::unique_ptr<MeshGeneration::TrianglesNodes>& trianglesNodes,
      Logger& logger);

   bool AddEdgeManifold1Intersections(
      const Geometry::IManifold1D2<MeshGeneration::GeomType>& manifold,
      const MeshGeneration::DirectedEdgeNodes& edge,
      const MeshGeneration::TrianglesNodes& trianglesNodes,
      MeshGeneration::ManifoldsAndNodes<GeomDim2>& manifoldsAndNodes,
      MeshGeneration::IUniquePointCollecion2& pointCollection);

   void AddManifold1Intersections(
      const Geometry::IManifold1D2<MeshGeneration::GeomType>& manifold,
      MeshGeneration::TrianglesNodes& trianglesNodes,
      MeshGeneration::ManifoldsAndNodes<GeomDim2>& manifoldsAndNodes,
      MeshGeneration::IUniquePointCollecion2& pointCollection,
      Logger& logger);

   void AddAllManifolds0(
      std::span<const IManifold0D2*> manifolds,
      MeshGeneration::TrianglesNodes& trianglesNodes,
      MeshGeneration::ManifoldsAndNodes<GeomDim2>& manifoldsAndNodes,
      MeshGeneration::IUniquePointCollecion2& pointCollection);

   std::unique_ptr<Vtk::VtkData> ToVtkData(const MeshGeneration::TrianglesNodes& trianglesNodes, const IPointCollection<MeshGeneration::GeomType, GeomDim2>& points);
}