#include "EdgeNodesDirected.h"
#include "Manifold1Reconstruction.h"
#include "Nibble.h"
#include "TrianglesNodes.h"

#include <set>

using namespace MeshGeneration;
using namespace Topology;

namespace
{
   std::set<const Geometry::IManifoldId*> getBoundaryManifoldIds(const Geometry::IGeometryRegion<MeshGeneration::GeomType, GeomDim2>& region)
   {
      std::set<const Geometry::IManifoldId*> boundaryManifolds;
      for (const auto* m : region.getManifolds().getBoundaryHyperManifolds())
      {
         boundaryManifolds.insert(m);
      }
      return boundaryManifolds;
   }

   std::vector<const MeshGeneration::IManifoldReconstruction*> getOuterBoundaryReconstructions(
      const Geometry::IGeometryRegion<MeshGeneration::GeomType, GeomDim2>& region,
      const std::vector<std::unique_ptr<MeshGeneration::IManifoldReconstruction>>& reconstructions)
   {
      const std::set<const Geometry::IManifoldId*> boundaryManifolds = getBoundaryManifoldIds(region);
      std::vector< const MeshGeneration::IManifoldReconstruction*> reconstructionPtrs(reconstructions.size());
      str::transform(reconstructions, reconstructionPtrs.begin(), [](const auto& up) {return up.get(); });
      std::vector<const MeshGeneration::IManifoldReconstruction*> result;
      str::copy_if(reconstructionPtrs, std::back_inserter(result), [&boundaryManifolds](const auto* ptr) {return boundaryManifolds.contains(&(ptr->getManifoldId())); });

      return result;
   }

   std::vector<EdgeNodesDirected> getBoundaryEdges(const std::vector<const MeshGeneration::IManifoldReconstruction*>& boundaryReconstructions)
   {
      std::vector<EdgeNodesDirected> boundaryEdges;
      for (const auto* br : boundaryReconstructions)
      {
         const auto* br1 = dynamic_cast<const Manifold1Reconstruction*>(br);
         for (const auto& path : br1->getReconstruction().Paths)
         {
            for (size_t n = 1; n < path.size(); ++n)
            {
               boundaryEdges.push_back({ path[n], path[n - 1] });
            }
         }
         for (const auto& cycle : br1->getReconstruction().Cycles)
         {
            for (size_t n = 1; n < cycle.size(); ++n)
            {
               boundaryEdges.push_back({ cycle[n], cycle[n - 1] });
            }
            boundaryEdges.push_back({ cycle.front(), cycle.back() });
         }
      }
      return boundaryEdges;
   }

   std::pair<CellIndex, CellIndex> getInsideAndOutsideCell(
      const EdgeNodesDirected& be,
      const MeshGeneration::TrianglesNodes& trianglesNodes,
      const IUniquePointCollection2& pointCollection,
      const Geometry::IGeometryRegion<MeshGeneration::GeomType, GeomDim2>& region)
   {
      const auto connectedTriangles = trianglesNodes.getEdgeConnectedTriangles(be[0], be[1]);
      if (connectedTriangles.size() != 2)
      {
         throw MyException("getBoundaryTriangles expect 2 triangles adjacent to edge");
      }
      const auto triangleNodes0 = trianglesNodes.getTriangleNodes(connectedTriangles.at(0));
      const auto triangleNodes1 = trianglesNodes.getTriangleNodes(connectedTriangles.at(1));
      const NodeIndex otherNode0 = triangleNodes0.oppositeNode(be);
      const NodeIndex otherNode1 = triangleNodes1.oppositeNode(be);
      const Point2 otherPoint0 = pointCollection.getPoint(otherNode0);
      const Point2 otherPoint1 = pointCollection.getPoint(otherNode1);
      const bool isContained0 = region.contains(otherPoint0, pointCollection.getGeometryPredicate());
      const bool isContained1 = region.contains(otherPoint1, pointCollection.getGeometryPredicate());
      if (isContained0 == isContained1)
      {
         throw MyException("getInsideAndOutsideCell both points inside or outside");
      }
      if (isContained0) return { connectedTriangles.at(0) , connectedTriangles.at(1) };
      return { connectedTriangles.at(1) , connectedTriangles.at(0) };
   }

   std::pair<std::set<CellIndex>, std::vector<CellIndex>> getBoundaryTriangles(
      const std::vector<EdgeNodesDirected>& boundaryEdges,
      const MeshGeneration::TrianglesNodes& trianglesNodes,
      const IUniquePointCollection2& pointCollection,
      const Geometry::IGeometryRegion<MeshGeneration::GeomType, GeomDim2>& region)
   {
      std::set<CellIndex> insideCells;
      std::vector<CellIndex> outsideCells;

      for (const auto& be : boundaryEdges)
      {
         auto [inside, outside] = getInsideAndOutsideCell(be, trianglesNodes, pointCollection, region);
         insideCells.insert(inside);
         outsideCells.push_back(outside);
      }
      return { insideCells, outsideCells };
   }

}

void MeshGeneration::nibble(
   const Geometry::IGeometryRegion<MeshGeneration::GeomType, GeomDim2>& region,
   const std::vector<std::unique_ptr<MeshGeneration::IManifoldReconstruction>>& reconstructions,
   MeshGeneration::TrianglesNodes& trianglesNodes,
   const IUniquePointCollection2& pointCollection,
   Logger& logger)
{
   const auto boundaryReconstructions = getOuterBoundaryReconstructions(region, reconstructions);
   const auto boundaryEdges = getBoundaryEdges(boundaryReconstructions);
   const auto [insideCells, outsideCells] = getBoundaryTriangles(boundaryEdges, trianglesNodes, pointCollection, region);
   std::vector<CellIndex> todo = outsideCells;
   while (!todo.empty())
   {
      const CellIndex cell = todo.back();
      todo.pop_back();
      if (trianglesNodes.isKnownTriangleId(cell))
      {
         const auto ngbs = trianglesNodes.getEdgeConnectedTriangles(cell);
         for (auto ngb : ngbs)
         {
            if (!insideCells.contains(ngb))
            {
               todo.push_back(ngb);
            }
         }
         trianglesNodes.deleteTriangle(cell);
      }
   }
}