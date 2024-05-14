#include "Manifold1Reconstruction.h"
#include "UndirectedGraph.h"

using namespace Manifold1Reconstruction;
using namespace MeshGeneration;

namespace
{
   std::unordered_map<NodeIndex, GraphVertex> RenumberToGraph(std::span<const NodeIndex> manifoldNodes)
   {
      std::unordered_map<NodeIndex, GraphVertex> result;
      for (GraphVertex v = 0; v < manifoldNodes.size(); ++v)
      {
         auto [itr, isnew] = result.emplace(manifoldNodes[v], v);
         if (!isnew) throw MyException("Duplicate node in manifoldNodes");
      }
      return result;
   }

   UndirectedGraph CreateGraph(std::span<const NodeIndex> manifoldNodes, const TrianglesNodes& trianglesNodes, const std::unordered_map<NodeIndex, GraphVertex>& toVertex)
   {
      UndirectedGraph result(manifoldNodes.size());
      std::vector<GraphVertex> neighbors;
      for (GraphVertex v = 0; v < manifoldNodes.size(); ++v)
      {
         const auto neighbors = trianglesNodes.getEdgeConnectedNodes(manifoldNodes[v]);
         if (neighbors.empty())
         {
            throw MyException("CreateGraph manifold node not found in triangulation");
         }
         for (auto ngb : neighbors)
         {
            result.addEdge(v, toVertex.at(ngb));
         }
      }
      return result;
   }
}

Manifold1Reconstruction::Reconstruction Manifold1Reconstruction::Generate2(std::span<const NodeIndex> manifoldNodes, const TrianglesNodes& trianglesNodes, IUniquePointCollecion2& pointCollection)
{
   Manifold1Reconstruction::Reconstruction result;
   const auto toVertex = RenumberToGraph(manifoldNodes);
   const auto graph = CreateGraph(manifoldNodes, trianglesNodes, toVertex);
   return result;
}