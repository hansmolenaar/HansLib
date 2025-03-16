#include "Manifold1Reconstruction.h"
#include "UndirectedGraph.h"

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
            const auto findNgb = toVertex.find(ngb);
            if (findNgb != toVertex.end())
            {
               result.addEdge(v, toVertex.at(ngb));
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
}

MeshGeneration::Reconstruction1 MeshGeneration::Generate2(std::span<const NodeIndex> manifoldNodes, const TrianglesNodes& trianglesNodes, const IUniquePointCollecion2& pointCollection)
{
   const auto toVertex = RenumberToGraph(manifoldNodes);
   const auto graph = CreateGraph(manifoldNodes, trianglesNodes, toVertex);
   for (GraphVertex v = 0; v < manifoldNodes.size(); ++v)
   {
      const auto degree = graph.getDegree(v);
      if (degree > 2)
      {
         throw MyException("Manifold1Reconstruction::Generate2 only max degree is 2 implemented, node " + std::to_string(manifoldNodes[v]) + " has degree " + std::to_string(degree));
      }
   }

   const auto cyclesAndPaths = graph.SplitInCyclesAndPaths();

   Reconstruction1 result;
   for (const auto& cycle : cyclesAndPaths.Cycles)
   {
      result.Cycles.emplace_back(ToNodesIndices(cycle, manifoldNodes));
   }
   for (const auto& path : cyclesAndPaths.Paths)
   {
      result.Paths.emplace_back(ToNodesIndices(path, manifoldNodes));
   }

   return result;
}

Manifold1Reconstruction::Manifold1Reconstruction(const Geometry::IManifoldId& manifoldId, const Reconstruction1& reconstruction) :
   m_manifoldId(manifoldId), m_reconstruction(reconstruction)
{
}

Manifold1Reconstruction::Manifold1Reconstruction(const Geometry::IManifold1D2<GeomType>& manifold, const ManifoldsAndNodes<GeomDim2>& manifoldsAndNodes,
   const TrianglesNodes& trianglesNodes, const IUniquePointCollecion2& pointCollection) :
   m_manifoldId(manifold),
   m_reconstruction(MeshGeneration::Generate2(manifoldsAndNodes.getNodesInManifold(&manifold), trianglesNodes, pointCollection))
{
}

const Geometry::IManifoldId& Manifold1Reconstruction::getManifoldId() const
{
   return m_manifoldId;
}

const Reconstruction1& Manifold1Reconstruction::getReconstruction() const
{
   return m_reconstruction;
}