#include "MeshGeneration2.h"
#include "IntervalTreeBalance.h"
#include "Defines.h"   
#include "BoundingBox.h"
#include "UniqueHashedPointCollection.h"
#include "UniquePointCollectionBinning.h"

using namespace Geometry;
using namespace IntervalTree;
using namespace MeshGeneration;

IndexTreeToSimplices2::Triangles MeshGeneration2::GenerateBaseTriangulation(const IGeometryRegion<GeomType, GeomDim2>& region, MeshingStrategy2& strategy, Logger& logger)
{
   std::string msg = "MeshGeneration2::GenerateBaseTriangulation() region Bb " + region.getBoundingBox().toString();
   logger.logLine(msg);

   IndexTree<GeomDim2> tree;
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
   const IGeometryPredicate<GeomType, GeomDim2>& predicate,
   const BoundingBox<GeomType, GeomDim2>& worldBB,
   std::unique_ptr<IDynamicUniquePointCollection<GeomType, GeomDim2>>& pointGeometry,
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

std::unique_ptr<Vtk::VtkData> MeshGeneration2::ToVtkData(const MeshGeneration::TrianglesNodes& triangleNodes, const IPointCollection<GeomType, GeomDim2>& points)
{
   constexpr int GeometryDimension = GeomDim2;
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

void MeshGeneration2::AddEdgeManifold1Intersections(
   const Geometry::IManifold1D2<MeshGeneration::GeomType>& manifold,
   const MeshGeneration::DirectedEdgeNodes& edgeNodes,
   const MeshGeneration::TrianglesNodes& trianglesNodes,
   MeshGeneration::ManifoldsAndNodes<GeomDim2>& manifoldsAndNodes,
   IUniquePointCollecion2& pointCollection)
{
   const auto& predicate = pointCollection.getGeometryPredicate();
   const DirectedEdge<GeomType, GeomDim2> edge(pointCollection.getPoint(edgeNodes[0]), pointCollection.getPoint(edgeNodes[1]));
   const auto intersections = manifold.GetIntersections(edge, predicate);
   if (intersections.empty()) return; // Nothing to do
   if (intersections.size() > 1)
   {
      throw MyException("Not yet implemented");
   }

   if (!std::holds_alternative<DirectedEdgePoint2>(intersections[0]))
   {
      throw MyException("Not yet implemented");
   }

   const auto& ip = std::get<DirectedEdgePoint2>(intersections[0]);
   if (ip.PointType != DirectedEdgePointType::Inside)
   {
      const auto node = (ip.PointType == DirectedEdgePointType::Point0 ? edgeNodes[0] : edgeNodes[1]);
      manifoldsAndNodes.addNodeToManifold(node, &manifold);
   }

   const auto dist0 = PointUtils::GetDistanceSquared(ip.EdgePoint, edge.point0());
   const auto dist1 = PointUtils::GetDistanceSquared(ip.EdgePoint, edge.point1());
   const auto nodeToMove = dist0 < dist1 ? edgeNodes[0] : edgeNodes[1];

   pointCollection.movePoint(nodeToMove, ip.EdgePoint);
   manifoldsAndNodes.addNodeToManifold(nodeToMove, &manifold);
}