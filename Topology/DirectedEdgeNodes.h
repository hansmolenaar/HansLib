#pragma once
#include "TopologyDefines.h"

namespace Topology
{
   //using DirectedEdgeNodes = std::array<NodeIndex, NumNodesOnEdge>;

   class DirectedEdgeNodes
   {
   public:
      DirectedEdgeNodes(NodeIndex n0, NodeIndex n1);
      NodeIndex operator[](size_t n) const { return m_nodes[n]; };
      auto begin() const { return m_nodes.begin(); }
      auto end() const { return m_nodes.end(); }
      auto operator<=>(const DirectedEdgeNodes& other) const { return m_nodes <=> other.m_nodes; };
      auto operator==(const DirectedEdgeNodes& other) const { return m_nodes == other.m_nodes; };

      bool contains(NodeIndex node) const;
   private:

      std::array<NodeIndex, NumNodesOnEdge> m_nodes;
   };
}