#pragma once

#include "EdgeNodesSorted.h"
#include "StreamUtils.h"
#include "TopologyDefines.h"

namespace Topology
{
   class TriangleNodes
   {
   public:
      explicit TriangleNodes(const std::array<NodeIndex, NumNodesOnTriangle>& nodes);
      TriangleNodes(NodeIndex n0, NodeIndex n1, NodeIndex n2);
      NodeIndex operator[](size_t n) const { return m_nodes[n]; };
      NodeIndex at(size_t n) const { return m_nodes[n]; };
      auto begin() const { return m_nodes.begin(); }
      auto end() const { return m_nodes.end(); }
      auto operator<=>(const TriangleNodes& other) const { return m_nodes <=> other.m_nodes; };
      auto operator==(const TriangleNodes& other) const { return m_nodes == other.m_nodes; };
      friend std::ostream& operator<<(std::ostream& os, const TriangleNodes& triangle)
      {
         return StreamUtils::insertList(os, triangle);
      }

      bool contains(NodeIndex node) const;
      bool containsNodes(NodeIndex node1, NodeIndex node2) const;
      NodeIndex oppositeNode(const EdgeNodesDirected& edge) const { return oppositeNode(EdgeNodesSorted(edge[0], edge[1])); };
      NodeIndex oppositeNode(const EdgeNodesSorted& edge) const;
      std::array<EdgeNodesDirected, NumEdgesOnTriangle> getEdges() const;

   private:
      std::array<NodeIndex, NumNodesOnTriangle> m_nodes;
   };


}