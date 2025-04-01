#pragma once

#include "EdgeNodesDirected.h"
#include "IDynamicUniquePointCollection.h"
#include "IGeometryRegion.h"
#include "IManifold0.h"
#include "IManifold1D2.h"
#include "IManifoldReconstruction.h"
#include "IMeshingSettings.h"
#include "IndexTreeToSimplices2.h"
#include "IRegionManifolds.h"
#include "Logger.h"
#include "ManifoldsAndNodes.h"
#include "MeshGenerationDefines.h"
#include "TrianglesNodes.h"
#include "VtkData.h"

namespace MeshGeneration2
{
   using IManifold0D2 = Geometry::IManifold0<MeshGeneration::GeomType, GeomDim2>;

   IndexTreeToSimplices2::Triangles GenerateBaseTriangulation(const Geometry::IGeometryRegion<MeshGeneration::GeomType, GeomDim2>& region, MeshGeneration::IMeshingSettings2& settings);

   void BaseTriangulationToWorld(
      const IndexTreeToSimplices2::Triangles& baseTriangles,
      const IGeometryPredicate<MeshGeneration::GeomType, GeomDim2>& predicate,
      const BoundingBox<MeshGeneration::GeomType, GeomDim2>& worldBB,
      std::unique_ptr<MeshGeneration::IUniquePointCollection2>& pointGeometry,
      MeshGeneration::TrianglesNodes& trianglesNodes,
      Logger& logger);

   bool AddEdgeManifold1Intersections(
      const Geometry::IManifold1D2<MeshGeneration::GeomType>& manifold,
      const Topology::EdgeNodesDirected& edge,
      const MeshGeneration::TrianglesNodes& trianglesNodes,
      MeshGeneration::ManifoldsAndNodes<GeomDim2>& manifoldsAndNodes,
      MeshGeneration::IUniquePointCollection2& pointCollection);

   // TODO add all at once
   void AddManifold1Intersections(
      const Geometry::IManifold1D2<MeshGeneration::GeomType>& manifold,
      MeshGeneration::TrianglesNodes& trianglesNodes,
      MeshGeneration::ManifoldsAndNodes<GeomDim2>& manifoldsAndNodes,
      MeshGeneration::IUniquePointCollection2& pointCollection,
      Logger& logger);

   void AddAllManifolds0(
      std::span<const IManifold0D2*> manifolds,
      MeshGeneration::TrianglesNodes& trianglesNodes,
      MeshGeneration::ManifoldsAndNodes<GeomDim2>& manifoldsAndNodes,
      MeshGeneration::IUniquePointCollection2& pointCollection,
      Logger& logger);

   std::vector<std::unique_ptr<MeshGeneration::IManifoldReconstruction>> createReconstructions(
      const Geometry::IRegionManifolds<MeshGeneration::GeomType, GeomDim2>& regionManifolds,
      const MeshGeneration::TrianglesNodes& trianglesNodes,
      const MeshGeneration::ManifoldsAndNodes<GeomDim2>& manifoldsAndNodes,
      const MeshGeneration::IUniquePointCollection2& pointCollection);

   bool checkReconstructions(
      const Geometry::IRegionManifolds<MeshGeneration::GeomType, GeomDim2>& regionManifolds,
      const std::vector<std::unique_ptr<MeshGeneration::IManifoldReconstruction>>& reconstructions,
      Logger& logger);

   std::unique_ptr<Vtk::VtkData> ToVtkData(const MeshGeneration::TrianglesNodes& trianglesNodes, const IPointCollection<MeshGeneration::GeomType, GeomDim2>& points, const Vtk::Name& name);
   std::vector<std::unique_ptr<Vtk::VtkData>> ToVtkData(const MeshGeneration::Reconstruction1& reconstruction, const IPointCollection<MeshGeneration::GeomType, GeomDim2>& points, const Vtk::Name& name);
   std::vector<std::unique_ptr<Vtk::VtkData>> ToVtkData(const std::vector<std::unique_ptr<MeshGeneration::IManifoldReconstruction>>& reconstructions,
      const IPointCollection<MeshGeneration::GeomType, GeomDim2>& points, const std::string& project);

}