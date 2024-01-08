#pragma once

#include<boost/container/static_vector.hpp>
#include <vector>
#include <unordered_map>
#include <array>
#include <optional>

namespace MeshGeneration
{
   class TriangleNodes
   {
   public:
      static constexpr size_t NumNodesOnEdge = 2;
      static constexpr size_t NumNodesOnTriangle = 3;

      using NodeId = int;
      using TriangleId = int;
      using SortedEdge = std::array<TriangleNodes::NodeId, NumNodesOnEdge>;

      // No ordering of the nodes assumed
      TriangleId addTriangle(NodeId n0, NodeId n1, NodeId n2);
      void deleteTriangle(TriangleId triangleId);
      boost::container::static_vector<TriangleId, 2> getEdgeConnectedTriangles(NodeId n0, NodeId n1) const;
      std::vector<TriangleId> getNodeConnectedTriangles(NodeId node) const;
      std::vector<NodeId> getEdgeConnectedNodes(NodeId node) const;
      std::optional<TriangleId> tryGetTriangle(NodeId n0, NodeId n1, NodeId n2) const;
      bool triangleContainsNode(TriangleId triangleId, NodeId nodeId) const;

      std::array<TriangleNodes::NodeId, TriangleNodes::NumNodesOnTriangle> getTriangleNodes(TriangleId triangle) const;

      bool isKnownNodeId(NodeId node) const;
      bool isKnownTriangleId(TriangleId triangle) const;

      std::vector<TriangleNodes::TriangleId> getAllTriangles() const;
      std::vector<TriangleNodes::SortedEdge> getAllSortedEdges() const;
   private:
      std::optional<TriangleNodes::TriangleId> tryGetTriangleFromSortedNodes(const std::array<NodeId, NumNodesOnTriangle>& nodes) const;
      void checkNodeId(NodeId nodeId) const;
      void checkTriangleId(TriangleId triangleId) const;

      std::unordered_map<TriangleId, std::array<TriangleNodes::NodeId, NumNodesOnTriangle>> m_toNodes;
      std::unordered_multimap<NodeId, TriangleNodes::TriangleId> m_toTriangles;
   };
}