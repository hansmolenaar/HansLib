#include "MeshGeneration2.h"
#include "IntervalTreeBalance.h"
#include "Defines.h"   
#include "BoundingBox.h"
#include "UniqueHashedPointCollection.h"
#include "UniquePointCollectionBinning.h"

using namespace Geometry;
using namespace IntervalTree;
using namespace MeshGeneration;

IndexTreeToSimplices2::Triangles MeshGeneration2::GenerateBaseTriangulation(const IGeometryRegion<double, 2>& region, MeshingStrategy2& strategy, Logger& logger)
{
   std::string msg = "MeshGeneration2::GenerateBaseTriangulation() region Bb " + region.getBoundingBox().toString();
   logger.logLine(msg);

   IndexTree<2> tree;
   tree.refineUntilReady(strategy.getRefinementPredicate());
   msg = "MeshGeneration2::GenerateBaseTriangulation() after refinement: " + tree.toString();
   logger.logLine(msg);

   IntervalTree::Balance(tree);
   msg = "MeshGeneration2::GenerateBaseTriangulation() after balancing (max 1 level difference): " + tree.toString();
   logger.logLine(msg);

   const auto result = IndexTreeToSimplices2::Create(tree);
   msg = "MeshGeneration2::GenerateBaseTriangulation(): number of triangles " + std::to_string(result.size());
   logger.logLine(msg);

   return result;
}

void MeshGeneration2::BaseTriangulationToWorld(
   const IndexTreeToSimplices2::Triangles& baseTriangles,
   const IGeometryPredicate<double, 2>& predicate,
   const BoundingBox<double, 2>& worldBB,
   std::unique_ptr<IDynamicUniquePointCollection<double, 2>>& pointGeometry,
   std::unique_ptr<MeshGeneration::TrianglesNodes>& triangleNodes,
   Logger& logger)
{
   // Clear output
   pointGeometry.reset();
   triangleNodes.reset();

   // Collect unique points
   std::unordered_map<RatPoint2, Point2> uniquePoints;
   std::vector<Point2> uniqueWorldPoints;
   for (auto& triangle : baseTriangles)
   {
      for (auto& p : triangle)
      {
         if (!uniquePoints.contains(p))
         {
            const Point2 worldPoint = worldBB.scaleFromPoint01(p);
            uniquePoints[p] = worldPoint;
            uniqueWorldPoints.push_back(worldPoint);
         }
      }
   }

   logger.logLine("MeshGeneration2::BaseTriangulationToWorld number of unique points: " + std::to_string(uniquePoints.size()));

   // Build the point collection from the points

   auto binnedCollection = std::make_unique<UniquePointCollectionBinning<2>>(predicate, uniqueWorldPoints);
   logger.logLine("MeshGeneration2::BaseTriangulationToWorld binning collecion: " + binnedCollection->toString());

   pointGeometry.reset(binnedCollection.release());

   std::unordered_map<RatPoint2, PointIndex> toWorld;

   // Get IDs for the points
   for (auto& up : uniquePoints)
   {
      const PointIndex worldId = *pointGeometry->tryGetClosePoint(up.second);
      toWorld.emplace(up.first, worldId);
   }



   // Create the triangles
   triangleNodes = std::make_unique<MeshGeneration::TrianglesNodes>();
   for (auto& triangle : baseTriangles)
   {
      const auto n0 = static_cast<PointIndex>(toWorld.at(triangle.at(0)));
      const auto n1 = static_cast<PointIndex>(toWorld.at(triangle.at(1)));
      const auto n2 = static_cast<PointIndex>(toWorld.at(triangle.at(2)));
      triangleNodes->addTriangle(n0, n1, n2);
   }

   logger.logLine("MeshGeneration2::BaseTriangulationToWorld topology\n" + triangleNodes->toString());
}

std::unique_ptr<Vtk::VtkData> MeshGeneration2::ToVtkData(const MeshGeneration::TrianglesNodes& triangleNodes, const IPointCollection<double, 2>& points)
{
   constexpr int GeometryDimension = 2;
   std::unique_ptr< Vtk::VtkData> result = std::make_unique<Vtk::VtkData>(GeometryDimension, 0);

   std::unordered_map<PointIndex, Vtk::NodeIndex> nodeToVtk;
   Vtk::NodeIndex nodeIndex = 0;
   for (auto& node : triangleNodes.getAllNodes())
   {
      const auto v = points.getPoint(node);
      std::array<Vtk::CoordinateType, GeometryDimension> coordinates{ static_cast<Vtk::CoordinateType>(v[0]),static_cast<Vtk::CoordinateType>(v[1]) };
      result->addNode(coordinates);
      nodeToVtk[node] = nodeIndex;
      ++nodeIndex;
   }

   for (const auto& cell : triangleNodes.getAllTriangles())
   {
      const auto tnodes = triangleNodes.getTriangleNodes(cell);
      const std::array<Vtk::NodeIndex, 3> vtkNodes{ nodeToVtk.at(tnodes.at(0)), nodeToVtk.at(tnodes.at(1)), nodeToVtk.at(tnodes.at(2)) };
      result->addCell(Vtk::CellType::VTK_TRIANGLE, vtkNodes, {});
   }

   return result;
}

void MeshGeneration2::InsertLineManifoldInTriangleByMovingPoints(
   const IManifold1D2<double>& manifold,
   const TriangleNodes& trianglesNodes,
   IDynamicUniquePointCollection<double, GeomDim2>& pointCollection)
{
   throw MyException("Not yet implemented");
}