#include "MeshGeneration2.h"
#include "IntervalTreeBalance.h"
#include "Defines.h"   
#include "BoundingBox.h"
#include "UniqueHashedPointCollection.h"
#include "UniquePointCollectionBinning.h"
#include "MyAssert.h"

#include <set>

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
            const auto worldPoint = worldBB.scaleFromPoint01(p);
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

static boost::container::static_vector< NodeIndex, 2> HandleEndPoints(
   DirectedEdgeIntersections<GeomType, GeomDim2>& intersections,
   const Geometry::IManifold1D2<MeshGeneration::GeomType>& manifold,
   const MeshGeneration::DirectedEdgeNodes& edgeNodes,
   MeshGeneration::ManifoldsAndNodes<GeomDim2>& manifoldsAndNodes)
{
   boost::container::static_vector< NodeIndex, 2> nodeUsed;
   if (intersections.empty()) return nodeUsed;
   for (int n = static_cast<int>(intersections.size()) - 1; n >= 0; --n)
   {
      const auto& ip = std::get<DirectedEdgePoint2>(intersections[n]);
      if (ip.PointType != DirectedEdgePointType::Inside)
      {
         const auto node = (ip.PointType == DirectedEdgePointType::Point0 ? edgeNodes[0] : edgeNodes[1]);
         nodeUsed.push_back(node);
         manifoldsAndNodes.addNodeToManifold(node, &manifold);
         intersections.erase(intersections.begin() + n);
      }
   }
   return nodeUsed;
}

// Return value: any node moved?
bool MeshGeneration2::AddEdgeManifold1Intersections(
   const Geometry::IManifold1D2<MeshGeneration::GeomType>& manifold,
   const MeshGeneration::DirectedEdgeNodes& edgeNodes,
   const MeshGeneration::TrianglesNodes& trianglesNodes,
   MeshGeneration::ManifoldsAndNodes<GeomDim2>& manifoldsAndNodes,
   IUniquePointCollecion2& pointCollection)
{
   const auto& predicate = pointCollection.getGeometryPredicate();
   const DirectedEdge<GeomType, GeomDim2> edge(pointCollection.getPoint(edgeNodes[0]), pointCollection.getPoint(edgeNodes[1]));
   auto intersections = manifold.GetIntersections(edge, predicate);
   if (intersections.empty()) return false; // Nothing to do

   if (!std::holds_alternative<DirectedEdgePoint2>(intersections[0]))
   {
      throw MyException("Intersecion interval not yet implemented");
   }

   const auto nodeUsed = HandleEndPoints(intersections, manifold, edgeNodes, manifoldsAndNodes);
   if (intersections.empty()) return false;

   if (intersections.size() == 1)
   {
      const auto& ip = std::get<DirectedEdgePoint2>(intersections[0]);
      const auto dist0 = PointUtils::GetDistanceSquared(ip.EdgePoint, edge.point0());
      const auto dist1 = PointUtils::GetDistanceSquared(ip.EdgePoint, edge.point1());
      auto nodeToMove = dist0 < dist1 ? edgeNodes[0] : edgeNodes[1];
      if (str::find(nodeUsed, nodeToMove) != nodeUsed.end())
      {
         // Take the other one
         nodeToMove = (nodeToMove == edgeNodes[0]) ? edgeNodes[1] : edgeNodes[0];
      }

      if (!manifoldsAndNodes.isMobileOnManifold(nodeToMove, &manifold))
      {
         throw MyException("MeshGeneration2::AddEdgeManifold1Intersections immovable node");
      }
      pointCollection.movePoint(nodeToMove, ip.EdgePoint);
      manifoldsAndNodes.addNodeToManifold(nodeToMove, &manifold);
   }
   else
   {
      Utilities::MyAssert(intersections.size() == 2);
      for (int n = 0; n < 2; ++n)
      {
         const auto& ip = std::get<DirectedEdgePoint2>(intersections[n]);
         // Automatically ordered: directed edge
         const auto nodeToMove = edgeNodes[n];
         if (!manifoldsAndNodes.isMobileOnManifold(nodeToMove, &manifold))
         {
            throw MyException("MeshGeneration2::AddEdgeManifold1Intersections immovable node");
         }
         pointCollection.movePoint(nodeToMove, ip.EdgePoint);
         manifoldsAndNodes.addNodeToManifold(nodeToMove, &manifold);
      }
   }
   return true;
}

void MeshGeneration2::AddManifold1Intersections(
   const Geometry::IManifold1D2<MeshGeneration::GeomType>& manifold,
   MeshGeneration::TrianglesNodes& trianglesNodes,
   MeshGeneration::ManifoldsAndNodes<GeomDim2>& manifoldsAndNodes,
   IUniquePointCollecion2& pointCollection)
{
   const auto initialSortedEdges = trianglesNodes.getAllSortedEdges();
   std::set<SortedEdgeNodes> todo(initialSortedEdges.begin(), initialSortedEdges.end());
   while (!todo.empty())
   {
      const auto edge = *todo.begin();
      todo.erase(edge);
      const bool anyNodeMoved = MeshGeneration2::AddEdgeManifold1Intersections(manifold, edge,
         trianglesNodes, manifoldsAndNodes, pointCollection);
      if (anyNodeMoved)
      {
         for (NodeIndex node : edge)
         {
            for (NodeIndex ngb : trianglesNodes.getEdgeConnectedNodes(node))
            {
               todo.insert(TrianglesNodes::CreateSortedEdge(node, ngb));
            }
         }
      }
   }
}