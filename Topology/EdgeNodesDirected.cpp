#include "EdgeNodesDirected.h"
#include "FirstDuplicate.h"

using namespace Topology;
using namespace Utilities;

EdgeNodesDirected::EdgeNodesDirected(NodeIndex n0, NodeIndex n1) :
   m_nodes{ n0, n1 }
{
   throwOnDuplicate(m_nodes);
   if (m_nodes[0] == NodeIndexInvalid || m_nodes[1] == NodeIndexInvalid)
   {
      throw MyException("DirectedEdgeNodes not valid: " + std::to_string(m_nodes[0]) + " and " + std::to_string(m_nodes[1]));
   }
}

bool EdgeNodesDirected::contains(NodeIndex node) const
{
   return node == m_nodes[0] || node == m_nodes[1];
}