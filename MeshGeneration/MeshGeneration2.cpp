#include "BoundingBox.h"
#include "Defines.h"   
#include "IntervalTreeBalance.h"
#include "Manifold1Reconstruction.h"
#include "MeshGeneration2.h"
#include "MeshGenerationUtils.h"
#include "MyAssert.h"
#include "Polygon2D.h"
#include "Triangle.h"
#include "UniqueHashedPointCollection.h"
#include "UniquePointCollectionBinning.h"

#include <set>

using namespace Geometry;
using namespace IntervalTree;
using namespace MeshGeneration;
using namespace MeshGeneration2;

namespace
{
   using M0ToTriIds = std::multimap<const IManifold0D2*, CellIndex>;
   M0ToTriIds GetTrianglesContainingPoints(
      std::span<const IManifold0D2*> manifolds,
      MeshGeneration::TrianglesNodes& trianglesNodes,
      MeshGeneration::IUniquePointCollecion2& pointCollection)
   {
      M0ToTriIds result;
      constexpr double eps = 0.01;
      const auto scale = BoundingBox<GeomType, GeomDim2>::CreateFromList(std::vector<Point2>{Point2{ -eps, -eps }, Point2{ 1 + eps, 1 + eps }});
      for (auto triangleId : trianglesNodes.getAllTriangles())
      {
         const auto triangle = GetTriangleGeometry(trianglesNodes.getTriangleNodes(triangleId), pointCollection);
         const auto bb = BoundingBox<GeomType, GeomDim2>::CreateFromList(triangle).scaleFrom01(scale);
         for (auto mptr : manifolds)
         {
            const auto point = mptr->GetPoint();
            if (bb.contains(point))
            {
               if (Polygon2D::Contains((std::span < const Point<GeomType, 2>>)triangle, point, pointCollection.getGeometryPredicate()))
               {
                  result.emplace(mptr, triangleId);
               }
            }
         }
      }
      return result;
   }

   // Explicit sorting, otherwise depence on pointer ordering
   std::vector<const IManifold0D2*> OrderManifolds0(std::span<const IManifold0D2*> manifolds)
   {
      std::vector<const IManifold0D2*> result(manifolds.begin(), manifolds.end());
      auto lessPoint = [](const IManifold0D2* ptr1, const IManifold0D2* ptr2) {return ptr1->GetPoint() < ptr2->GetPoint(); };
      str::sort(result, lessPoint);
      return result;
   }

   NodeIndex InsertPoint(Point2 point, std::span<const CellIndex> candidates, TrianglesNodes& trianglesNodes, MeshGeneration::IUniquePointCollecion2& pointCollection, const MeshGeneration::ManifoldsAndNodes<GeomDim2>& manifoldsAndNodes)
   {
      if (candidates.size() != 1) throw MyException("Not yet implemented");
      return -1;
   }
}


IndexTreeToSimplices2::Triangles MeshGeneration2::GenerateBaseTriangulation(const Geometry::IGeometryRegion<MeshGeneration::GeomType, GeomDim2>& region, IMeshingSettings2& settings)
{
   auto& logger = settings.getLogger();
   std::string msg = "MeshGeneration2::GenerateBaseTriangulation() region Bb " + region.getBoundingBox().toString();
   logger.logLine(msg);

   IndexTree<GeomDim2> tree;
   auto refinementPredicate = settings.getRefinementPredicateFactory().Create(region, settings.getInitialBbGenerator(), settings.getGeometryPredicate());
   tree.refineUntilReady(*refinementPredicate);
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
   std::unique_ptr<IUniquePointCollecion2>& pointGeometry,
   std::unique_ptr<TrianglesNodes>& triangleNodes,
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
   triangleNodes = std::make_unique<TrianglesNodes>();
   for (auto& triangle : baseTriangles)
   {
      const auto n0 = static_cast<PointIndex>(toWorld.at(triangle.at(0)));
      const auto n1 = static_cast<PointIndex>(toWorld.at(triangle.at(1)));
      const auto n2 = static_cast<PointIndex>(toWorld.at(triangle.at(2)));
      triangleNodes->addTriangle(n0, n1, n2);
   }

   logger.logLine("MeshGeneration2::BaseTriangulationToWorld topology\n" + triangleNodes->toString());
}

std::unique_ptr<Vtk::VtkData> MeshGeneration2::ToVtkData(const TrianglesNodes& triangleNodes, const IPointCollection<GeomType, GeomDim2>& points, const Vtk::Name& name)
{
   std::unique_ptr< Vtk::VtkData> result = std::make_unique<Vtk::VtkData>(GeomDim2, 0, name);

   for (const auto& cell : triangleNodes.getAllTriangles())
   {
      const auto tnodes = triangleNodes.getTriangleNodes(cell);
      result->addCell(Vtk::CellType::VTK_TRIANGLE, tnodes, points, {});
   }

   return result;
}

std::vector<std::unique_ptr<Vtk::VtkData>> MeshGeneration2::ToVtkData(
   const Manifold1Reconstruction::Reconstruction& reconstruction,
   const IPointCollection<MeshGeneration::GeomType, GeomDim2>& points,
   const Vtk::Name& name)
{
   std::vector<std::unique_ptr<Vtk::VtkData>> result;
   if (!reconstruction.Singletons.empty()) throw MyException("MeshGeneration2::ToVtkData unexpected singletons");

   // Collect Vtk nodes
   std::unordered_map<PointIndex, Vtk::NodeIndex> nodeToVtk;
   Vtk::NodeIndex nodeIndex = 0;
   for (int count = 0; const auto & path : reconstruction.Paths)
   {
      ++count;
      const Vtk::Name  vtkName{ name.project, name.item + "_path_" + std::to_string(count) };
      std::unique_ptr< Vtk::VtkData> part = std::make_unique< Vtk::VtkData>(GeomDim2, 0, vtkName);
      for (size_t n = 1; n < path.size(); ++n)
      {
         std::array<PointIndex, 2> edge{ path[n], path[n - 1] };
         part->addCell(Vtk::CellType::VTK_LINE, edge, points, {});
      }
      result.emplace_back(std::move(part));
   }

   nodeIndex = 0;
   for (int count = 0; const auto & cycle : reconstruction.Cycles)
   {
      ++count;
      const Vtk::Name  vtkName{ name.project, name.item + "_cycle_" + std::to_string(count) };
      std::unique_ptr< Vtk::VtkData> part = std::make_unique< Vtk::VtkData>(GeomDim2, 0, vtkName);
      for (size_t n = 0; n < cycle.size(); ++n)
      {
         std::array<PointIndex, 2> edge{ cycle[n], cycle[(n + 1) % cycle.size()] };
         part->addCell(Vtk::CellType::VTK_LINE, edge, points, {});
      }
      result.emplace_back(std::move(part));
   }

   return result;
}

static boost::container::static_vector< NodeIndex, 2> HandleEndPoints(
   DirectedEdgeIntersections<GeomType, GeomDim2>& intersections,
   const Geometry::IManifold1D2<GeomType>& manifold,
   const DirectedEdgeNodes& edgeNodes,
   ManifoldsAndNodes<GeomDim2>& manifoldsAndNodes)
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
         intersections.m_data.erase(intersections.m_data.begin() + n);
      }
   }
   return nodeUsed;
}

// Return value: any node moved?
bool MeshGeneration2::AddEdgeManifold1Intersections(
   const Geometry::IManifold1D2<GeomType>& manifold,
   const DirectedEdgeNodes& edgeNodes,
   const TrianglesNodes& trianglesNodes,
   ManifoldsAndNodes<GeomDim2>& manifoldsAndNodes,
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
   const Geometry::IManifold1D2<GeomType>& manifold,
   TrianglesNodes& trianglesNodes,
   MeshGeneration::ManifoldsAndNodes<GeomDim2>& manifoldsAndNodes,
   IUniquePointCollecion2& pointCollection,
   Logger& logger)
{
   //LogTriangles(logger, trianglesNodes, "trianglesNodes");
   const auto initialSortedEdges = trianglesNodes.getAllSortedEdges();
   //Log(logger, initialSortedEdges, "initialSortedEdges "); 
   std::set<SortedEdgeNodes> todo(initialSortedEdges.begin(), initialSortedEdges.end());
   while (!todo.empty())
   {
      const auto edge = *todo.begin();
      //LogSortedEdgeNodes(logger, edge, "TODO handle edge: ");
      todo.erase(edge);
      const bool anyNodeMoved = AddEdgeManifold1Intersections(manifold, edge,
         trianglesNodes, manifoldsAndNodes, pointCollection);
      if (anyNodeMoved)
      {
         for (NodeIndex node : edge)
         {
            for (NodeIndex ngb : trianglesNodes.getEdgeConnectedNodes(node))
            {
               const auto todoEdge = TrianglesNodes::CreateSortedEdge(node, ngb);
               todo.insert(todoEdge);
               //LogSortedEdgeNodes(logger, todoEdge, "Add to TODO ");
            }
         }
      }
   }
}

void MeshGeneration2::AddAllManifolds0(
   std::span<const IManifold0D2*> manifolds,
   MeshGeneration::TrianglesNodes& trianglesNodes,
   MeshGeneration::ManifoldsAndNodes<GeomDim2>& manifoldsAndNodes,
   MeshGeneration::IUniquePointCollecion2& pointCollection)
{
   // Collect triangles that contain point
   const auto trianglesContainingPoints = GetTrianglesContainingPoints(manifolds, trianglesNodes, pointCollection);

   for (const auto* manifold : OrderManifolds0(manifolds))
   {
      const auto range = trianglesContainingPoints.equal_range(manifold);
      std::vector<CellIndex> candidates;
      std::transform(range.first, range.second, std::back_inserter(candidates), [](const auto& itr) {return itr.second; });
      const NodeIndex node = InsertPoint(manifold->GetPoint(), candidates, trianglesNodes, pointCollection, manifoldsAndNodes);
      manifoldsAndNodes.addNodeToManifold(node, manifold);
   }

   throw MyException("Not yet implemented");
}