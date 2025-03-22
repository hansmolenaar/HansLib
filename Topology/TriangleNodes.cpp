#include "MyException.h"
#include "TriangleNodes.h"

#include <sstream>

Topology::TriangleNodes::TriangleNodes(NodeIndex n0, NodeIndex n1, NodeIndex n2) :
   m_nodes{ n0, n1, n2 }
{
   if (n0 == n1 || n0 == n2 || n1 == n2)
   {
      std::ostringstream os;
      os << *this;
      throw MyException("TriangleNodes invalid: " + os.str());
   }
}

Topology::TriangleNodes::TriangleNodes(const std::array<NodeIndex, NumNodesOnTriangle>& nodes) :
   TriangleNodes(nodes[0], nodes[1], nodes[2])
{
}

bool Topology::TriangleNodes::contains(NodeIndex node) const
{
   return node == m_nodes[0] || node == m_nodes[1] || node == m_nodes[2];
}

bool Topology::TriangleNodes::contains(const EdgeNodesDirected& edge) const
{
   return contains(edge[0]) && contains(edge[1]);
}

Topology::NodeIndex Topology::TriangleNodes::oppositeNode(const EdgeNodesDirected& edge) const
{
   if (!edge.contains(m_nodes[0]))
   {
      if (!edge.contains(m_nodes[1]) || !edge.contains(m_nodes[2]))
      {
         throw MyException("Triangle does not contain edge");
      }
      return  m_nodes[0];
   }
   else if (!edge.contains(m_nodes[1]))
   {

      if (!edge.contains(m_nodes[2]))
      {
         throw MyException("Triangle does not contain edge");
      }
      return m_nodes[1];
   }
   else if (!edge.contains(m_nodes[2]))
   {
      return  m_nodes[2];
   }

   throw MyException("TriangleNodesNodesOppositeNode should not get here");
}