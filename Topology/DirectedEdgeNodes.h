#pragma once
#include "TopologyDefines.h"
#include <ostream>

namespace Topology
{
   class DirectedEdgeNodes
   {
   public:
      DirectedEdgeNodes(NodeIndex n0, NodeIndex n1);
      NodeIndex operator[](size_t n) const { return m_nodes[n]; };
      auto begin() const { return m_nodes.begin(); }
      auto end() const { return m_nodes.end(); }
      auto operator<=>(const DirectedEdgeNodes& other) const { return m_nodes <=> other.m_nodes; };
      auto operator==(const DirectedEdgeNodes& other) const { return m_nodes == other.m_nodes; };
      friend std::ostream& operator<<(std::ostream& os, const DirectedEdgeNodes& edge)
      {
         os << "(" << edge[0] << " -> " << edge[1] << ")";
         return os;
      }
      bool contains(NodeIndex node) const;
   private:

      std::array<NodeIndex, NumNodesOnEdge> m_nodes;
   };
}