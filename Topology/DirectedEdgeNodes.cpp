#include "DirectedEdgeNodes.h"

Topology::DirectedEdgeNodes::DirectedEdgeNodes(NodeIndex n0, NodeIndex n1) :
   m_nodes{ n0, n1 }
{
   if (m_nodes[0] == m_nodes[1] || m_nodes[0] == NodeIndexInvalid || m_nodes[1] == NodeIndexInvalid)
   {
      throw MyException("DirectedEdgeNodes not valid: " + std::to_string(m_nodes[0]) + " and " + std::to_string(m_nodes[1]));
   }
}

bool Topology::DirectedEdgeNodes::contains(NodeIndex node) const
{
   return node == m_nodes[0] || node == m_nodes[1];
}