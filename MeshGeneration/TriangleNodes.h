#pragma once

#include<boost/container/static_vector.hpp>
#include <vector>
#include <unordered_map>
#include <array>

namespace MeshGeneration
{
   class TriangleNodes
   {
   public:
      using NodeId = int;
      using TriangleId = int;

      static constexpr size_t NumNodesOnEdge = 2;
      static constexpr size_t NumNodesOnTriangle = 3;

      // No ordering of the nodes assumed
      TriangleId addTriangle(NodeId n0, NodeId n1, NodeId n2);
      void deleteTriangle(TriangleId triangleId);
      boost::container::static_vector<TriangleId, 2> getEdgeConnectedTriangles(NodeId n0, NodeId n1) const;
      std::vector<TriangleId> getNodeConnectedTriangles(NodeId node) const;
      std::vector<NodeId> getEdgeConnectedNodes(NodeId node) const;
      std::pair<bool, TriangleId> tryGetTriangle(NodeId n0, NodeId n1, NodeId n2) const;
      bool triangleContainsNode(TriangleId triangleId, NodeId nodeId) const;

   private:
      std::pair<bool, TriangleNodes::TriangleId> tryGetTriangleFromSortedNodes(const std::array<NodeId, NumNodesOnTriangle>& nodes) const;
      void checkNodeId(NodeId nodeId) const;
      void checkTriangleId(TriangleId triangleId) const;

      std::unordered_map<TriangleId, std::array<TriangleNodes::NodeId, NumNodesOnTriangle>> m_toNodes;
      std::unordered_multimap<NodeId, TriangleNodes::TriangleId> m_toTriangles;
   };
}