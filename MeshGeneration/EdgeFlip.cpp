#include "EdgeFlip.h"
#include "Manifold1Reconstruction.h"
#include "TrianglesNodes.h"

#include <queue>

using namespace Geometry;
using namespace MeshGeneration;
using namespace Topology;

namespace
{
   struct CellAndQuality
   {
      CellIndex triangleId;
      double quality;
      bool operator<(const CellAndQuality& other) const { return quality > other.quality; }
   };

   struct EdgeFlipData
   {
      EdgeNodesDirected edge = { 0, 1 };
      CellIndex otherCell = CellIndexInvalid;
      double minQualityAfterFlip = std::numeric_limits<double>::min();
      TriangleNodes newCell0{ 0,1,2 };
      TriangleNodes newCell1{ 0,1,2 };
   };

   double getTriangleQuality(const TriangleNodes& triangleNodes, const IUniquePointCollection2& pointCollection, CellQuality2Fun* cellQuality)
   {
      std::array<Point2, Topology::NumNodesOnTriangle> triangle;
      str::transform(triangleNodes, triangle.begin(), [&pointCollection](NodeIndex node) {return pointCollection.getPoint(node); });
      return cellQuality(triangle);
   }

   CellAndQuality getTriangleAndQuality(const CellIndex& cellId, const TrianglesNodes& trianglesNodes, const IUniquePointCollection2& pointCollection, CellQuality2Fun* cellQuality)
   {
      const auto triangleNodes = trianglesNodes.getTriangleNodes(cellId);
      return CellAndQuality{ cellId, getTriangleQuality(triangleNodes.asTriangleNodes(), pointCollection, cellQuality) };
   }

   template<typename Pred>
   EdgeFlipData getOptimalFlip(CellIndex cell, const TrianglesNodes& trianglesNodes, const IUniquePointCollection2& pointCollection, CellQuality2Fun* cellQuality, Pred isFlippable)
   {
      EdgeFlipData optimalEdge;
      const auto cellNodes = trianglesNodes.getTriangleNodes(cell);
      const auto neighbors = trianglesNodes.getEdgeConnectedTriangles(cell);
      for (CellIndex ngb : neighbors)
      {
         const auto commonNodes = trianglesNodes.getCommonNodes(cell, ngb);
         if (commonNodes.size() != 2) continue;
         const EdgeNodesDirected edge{ commonNodes[0], commonNodes[1] };
         if (!isFlippable(EdgeNodesSorted(edge))) continue;

         const auto ngbNodes = trianglesNodes.getTriangleNodes(ngb);
         const auto oppNodeCell = cellNodes.oppositeNode(edge);
         const auto oppNodeNgb = ngbNodes.oppositeNode(edge);
         const TriangleNodes cell0{ edge[0], oppNodeNgb, oppNodeCell };
         const TriangleNodes cell1{ edge[1], oppNodeNgb, oppNodeCell };
         const double quality0 = getTriangleQuality(cell0, pointCollection, cellQuality);
         const double quality1 = getTriangleQuality(cell1, pointCollection, cellQuality);
         const double minQuality = std::min(quality0, quality1);
         if (minQuality > optimalEdge.minQualityAfterFlip)
         {
            optimalEdge = { edge, ngb, minQuality, cell0, cell1 };
         }
      }

      return optimalEdge;
   }
}

MeshGeneration::EdgeFlip::EdgeFlip(
   TrianglesNodes& trianglesNodes,
   CellQuality2Fun* getCellQuality,
   const IUniquePointCollection2& pointCollection,
   const std::vector<std::unique_ptr<IManifoldReconstruction>>& reconstructions) :
   m_trianglesNodes(trianglesNodes),
   m_cellQuality(getCellQuality),
   m_pointCollection(pointCollection),
   m_reconstructions(reconstructions.size())
{
   str::transform(reconstructions, m_reconstructions.begin(), [](const std::unique_ptr<IManifoldReconstruction>& up) {return up.get(); });
}

void MeshGeneration::EdgeFlip::execute(const EdgeFlipStrategy& strategy)
{
   if (strategy.maxNumSweeps <= 0) return;
   std::priority_queue<CellAndQuality> taq;

   std::vector<CellIndex> todo = m_trianglesNodes.getAllTriangles();
   for (int sweep = 0; sweep < strategy.maxNumSweeps; ++sweep)
   {
      for (auto c : todo)
      {
         if (m_trianglesNodes.isKnownTriangleId(c))
         {
            const auto triangleAndQuality = getTriangleAndQuality(c, m_trianglesNodes, m_pointCollection, m_cellQuality);
            // forget about the bulk of the cells
            if (triangleAndQuality.quality < strategy.treshold)
            {
               taq.push(triangleAndQuality);
            }
         }
      }

      todo.clear();
      while (!taq.empty())
      {
         const auto poorTriangle = taq.top();
         taq.pop();
         if (!m_trianglesNodes.isKnownTriangleId(poorTriangle.triangleId))
         {
            continue;
         }

         auto canBeFlipped = [this](const Topology::EdgeNodesSorted& edge) {return isFlippable(edge); };
         const auto edgeFlipData = getOptimalFlip(poorTriangle.triangleId, m_trianglesNodes, m_pointCollection, m_cellQuality, canBeFlipped);
         if (edgeFlipData.otherCell == CellIndexInvalid || edgeFlipData.minQualityAfterFlip < poorTriangle.quality)
         {
            // Nothing we can do yet
            todo.push_back(poorTriangle.triangleId);
            continue;
         }
         else
         {
            m_trianglesNodes.deleteTriangle(poorTriangle.triangleId);
            m_trianglesNodes.deleteTriangle(edgeFlipData.otherCell);
            const auto newCellId0 = m_trianglesNodes.addTriangle(edgeFlipData.newCell0);
            const auto newCellId1 = m_trianglesNodes.addTriangle(edgeFlipData.newCell1);
            todo.push_back(newCellId0);
            todo.push_back(newCellId1);
         }
      }
   }
}

bool MeshGeneration::EdgeFlip::isFlippable(const EdgeNodesSorted& edge) const
{
   for (const auto* r : m_reconstructions)
   {
      const auto* r1 = dynamic_cast<const Manifold1Reconstruction*>(r);
      if (r1 == nullptr) continue;

      if (r1->contains(edge)) return false;
   }
   return true;
}