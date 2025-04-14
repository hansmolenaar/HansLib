#include "Boundary1.h"
#include "UndirectedGraph.h"

using namespace MeshGeneration;
using namespace Topology;

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

   UndirectedGraph CreateGraph(const TrianglesNodes& trianglesNodes, const std::unordered_map<NodeIndex, GraphVertex>& toVertex)
   {
      UndirectedGraph result(toVertex.size());
      std::vector<GraphVertex> neighbors;
      for (const auto& vert : toVertex)
      {
         const auto neighbors = trianglesNodes.getEdgeConnectedNodes(vert.first);
         if (neighbors.empty())
         {
            throw MyException("CreateGraph manifold node not found in triangulation");
         }
         for (auto ngb : neighbors)
         {
            const auto findNgb = toVertex.find(ngb);
            if (findNgb != toVertex.end())
            {
               result.addEdge(vert.second, toVertex.at(ngb));
            }
         }
      }
      return result;
   }

   std::vector<NodeIndex> ToNodesIndices(const std::vector<GraphVertex>& grapVertices, std::span<const NodeIndex> manifoldNodes)
   {
      std::vector<NodeIndex> result(manifoldNodes.size());
      str::transform(grapVertices, result.begin(), [&manifoldNodes](GraphVertex vertex) {return manifoldNodes[vertex]; });
      return result;
   }

   std::set<Topology::EdgeNodesSorted> getEdges(const Boundary1 reconstruction)
   {
      std::set<Topology::EdgeNodesSorted> result;
      for (const auto& path : reconstruction.Paths)
      {
         const size_t siz = path.size();
         for (size_t n = 1; n < siz; ++n)
         {
            result.emplace(path[n - 1], path[n]);
         }
      }

      for (const auto& cycle : reconstruction.Cycles)
      {
         const size_t siz = cycle.size();
         for (size_t n = 1; n <= siz; ++n)
         {
            result.emplace(cycle[n - 1], cycle[n % siz]);
         }
      }

      return result;
   }
}

MeshGeneration::Boundary1::Boundary1(std::span<const NodeIndex> activeNodes, const TrianglesNodes& trianglesNodes)
{
   const auto toVertex = RenumberToGraph(activeNodes);
   const auto graph = CreateGraph(trianglesNodes, toVertex);
   for (GraphVertex v = 0; v < toVertex.size(); ++v)
   {
      const auto degree = graph.getDegree(v);
      if (degree > 2)
      {
         throw MyException("Manifold1Reconstruction::Generate2 only max degree is 2 implemented, node " + std::to_string(activeNodes[v]) + " has degree " + std::to_string(degree));
      }
   }

   const auto cyclesAndPaths = graph.SplitInCyclesAndPaths();

   for (const auto& cycle : cyclesAndPaths.Cycles)
   {
      Cycles.emplace_back(ToNodesIndices(cycle, activeNodes));
   }
   for (const auto& path : cyclesAndPaths.Paths)
   {
      Paths.emplace_back(ToNodesIndices(path, activeNodes));
   }
}
