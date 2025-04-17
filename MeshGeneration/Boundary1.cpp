#include "Boundary1.h"
#include "RenumberContiguous.h"
#include "UndirectedGraph.h"

using namespace MeshGeneration;
using namespace Topology;
using namespace Utilities;

namespace
{
   UndirectedGraph CreateGraph(std::span<const Topology::EdgeNodesSorted> edges, const RenumberContiguous<NodeIndex, GraphVertex>& toVertex)
   {
      UndirectedGraph result(toVertex.size());
      for (const auto& edge : edges)
      {
         result.addEdge(*toVertex.toContiguous(edge[0]), *toVertex.toContiguous(edge[1]));
      }
      return result;
   }

   UndirectedGraph CreateGraph(const TrianglesNodes& trianglesNodes, const RenumberContiguous<NodeIndex, GraphVertex>& toVertex)
   {
      std::vector< EdgeNodesSorted> edges;

      for (GraphVertex n = 0; n < toVertex.size(); ++n)
      {
         const NodeIndex node = toVertex.at(n);
         const auto neighbors = trianglesNodes.getEdgeConnectedNodes(node);
         if (neighbors.empty())
         {
            throw MyException("CreateGraph manifold node not found in triangulation");
         }
         for (auto ngb : neighbors)
         {
            if (toVertex.toContiguous(ngb))
            {
               edges.emplace_back(node, ngb);
            }
         }
      }
      return CreateGraph(edges, toVertex);
   }

   std::vector<NodeIndex> ToNodesIndices(const std::vector<GraphVertex>& grapVertices, std::span<const NodeIndex> manifoldNodes)
   {
      std::vector<NodeIndex> result(manifoldNodes.size());
      str::transform(grapVertices, result.begin(), [&manifoldNodes](GraphVertex vertex) {return manifoldNodes[vertex]; });
      return result;
   }
}

MeshGeneration::Boundary1::Boundary1(const std::vector<Topology::EdgeNodesSorted>& edgeSet, std::vector<Topology::NodeIndex> activeNodes)
{
   for (const auto& edge : edgeSet)
   {
      activeNodes.push_back(edge[0]);
      activeNodes.push_back(edge[1]);
   }
   str::sort(activeNodes);
   activeNodes.erase(std::unique(activeNodes.begin(), activeNodes.end()), activeNodes.end());

   const RenumberContiguous<NodeIndex, GraphVertex> toVertex(activeNodes.begin(), activeNodes.end());
   const UndirectedGraph graph = CreateGraph(edgeSet, toVertex);

   const auto cyclesAndPaths = graph.SplitInCyclesAndPaths();

   for (const auto& cycle : cyclesAndPaths.Cycles)
   {
      m_cycles.emplace_back(ToNodesIndices(cycle, activeNodes));
   }
   for (const auto& path : cyclesAndPaths.Paths)
   {
      m_paths.emplace_back(ToNodesIndices(path, activeNodes));
   }

   for (auto v : graph.getIsolatedVertices())
   {
      m_singletons.push_back(activeNodes[v]);
   }
}

MeshGeneration::Boundary1 Boundary1::createFromSubSet(const std::vector<Topology::NodeIndex>& activeNodes, const TrianglesNodes& trianglesNodes)
{
   std::set<NodeIndex> activeNodeSet(activeNodes.begin(), activeNodes.end());
   std::vector<Topology::EdgeNodesSorted> edges;
   for (const auto& edge : trianglesNodes.getAllSortedEdges())
   {
      if (activeNodeSet.contains(edge[0]) && activeNodeSet.contains(edge[1]))
      {
         edges.push_back(edge);
      }
   }
   return Boundary1(edges, activeNodes);
}

const std::vector<Topology::NodeIndex>& Boundary1::getSingletons() const
{
   return m_singletons;
}

const std::vector<PathNodes>& Boundary1::getPaths() const
{
   return m_paths;
}

const std::vector<CycleNodes>& Boundary1::getCycles() const
{
   return m_cycles;
}

bool Boundary1::empty() const
{
   return  m_singletons.empty() && m_paths.empty() && m_cycles.empty();
}

Boundary1 Boundary1::createFromBoundaryEdges(const TrianglesNodes& trianglesNodes)
{
   std::vector<EdgeNodesSorted> activeEdges;
   str::copy_if(trianglesNodes.getAllSortedEdges(), std::back_inserter(activeEdges), [&trianglesNodes](const auto& edge) {
      return trianglesNodes.getTrianglesContainingEdge(edge[0], edge[1]).size() == 1; });
   return Boundary1(activeEdges);
}

std::set<Topology::EdgeNodesSorted> Boundary1::getEdges() const
{
   std::set<Topology::EdgeNodesSorted> result;
   for (const auto& path : getPaths())
   {
      for (const auto& edge : path.getEdges())
      {
         result.emplace(edge[0], edge[1]);
      }
   }

   for (const auto& cycle : getCycles())
   {
      for (const auto& edge : cycle.getEdges())
      {
         result.emplace(edge[0], edge[1]);
      }
   }

   return result;
}