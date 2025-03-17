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

   std::vector<std::array<NodeIndex, NumNodesOnEdge>> getBoundaryEdges(const std::vector<const MeshGeneration::IManifoldReconstruction*>& boundaryReconstructions)
   {
      std::vector<std::array<NodeIndex, NumNodesOnEdge>> boundaryEdges;
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

   std::vector<CellIndex> getBoundaryTriangles(const std::vector<std::array<NodeIndex, NumNodesOnEdge>>& boundaryEdges, const MeshGeneration::TrianglesNodes& trianglesNodes)
   {
      std::vector<CellIndex> result;
      result.reserve(boundaryEdges.size() * 2);
      for (const auto& be : boundaryEdges)
      {
         const auto connectedTriangles = trianglesNodes.getEdgeConnectedTriangles(be[0], be[1]);
         result.insert(result.end(), connectedTriangles.begin(), connectedTriangles.end());
      }
      // remove duplicates
      std::sort(result.begin(), result.end());
      result.erase(unique(result.begin(), result.end()), result.end());
      return result;
   }

   std::pair<std::set<CellIndex>, std::vector<CellIndex>> splitInsideOutside(
      const std::vector<CellIndex>& boundaryCells,
      const MeshGeneration::IUniquePointCollection2& pointCollection,
      const Geometry::IGeometryRegion<MeshGeneration::GeomType, GeomDim2>& region)
   {
      return{};
   }
}

void MeshGeneration::nibble(
   const Geometry::IGeometryRegion<MeshGeneration::GeomType, GeomDim2>& region,
   const std::vector<std::unique_ptr<MeshGeneration::IManifoldReconstruction>>& reconstructions,
   const MeshGeneration::TrianglesNodes& trianglesNodes,
   const MeshGeneration::IUniquePointCollection2& pointCollection,
   Logger& logger)
{
   const auto boundaryReconstructions = getOuterBoundaryReconstructions(region, reconstructions);
   const auto boundaryEdges = getBoundaryEdges(boundaryReconstructions);
   const auto boundaryTriangles = getBoundaryTriangles(boundaryEdges, trianglesNodes);
}