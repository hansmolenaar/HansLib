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
      NodeIndex& operator[](size_t n) { return m_nodes[n]; };
      bool contains(NodeIndex node) const;
   private:

      std::array<NodeIndex, NumNodesOnEdge> m_nodes;
   };
}