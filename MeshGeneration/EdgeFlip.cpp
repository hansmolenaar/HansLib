#include "EdgeFlip.h"
#include "Manifold1Reconstruction.h"
#include "TrianglesNodes.h"

#include <queue>

using namespace Geometry;
using namespace MeshGeneration;

namespace
{
   struct CellAndQuality
   {
      CellIndex triangleId;
      double quality;
      bool operator<(const CellAndQuality& other) const { return quality > other.quality; }
   };

   CellAndQuality getTriangleAndQuality(const CellIndex& cellId, const TrianglesNodes& trianglesNodes, const IUniquePointCollection2& pointCollection, CellQuality2Fun* cellQuality)
   {
      const auto triangleNodes = trianglesNodes.getTriangleNodes(cellId);
      std::array<Point2, Topology::NumNodesOnTriangle> triangle;
      str::transform(triangleNodes, triangle.begin(), [&pointCollection](NodeIndex node) {return pointCollection.getPoint(node); });
      return CellAndQuality{ cellId, cellQuality(triangle) };
   }

   CellIndex getOptimalFlip(CellIndex cell)
   {
      return CellIndexInvalid;
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
         const auto flippedNeighbor = getOptimalFlip(poorTriangle.triangleId);
         if (flippedNeighbor == CellIndexInvalid)
         {
            todo.push_back(poorTriangle.triangleId);
            continue;
         }
      }
   }
}