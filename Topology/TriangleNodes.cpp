#include "ITopologicalAdjacency.h"
#include "MyException.h"
#include "ReferenceShapePolygon.h"
#include "TriangleNodes.h"

#include <sstream>

using namespace Topology;

TriangleNodes::TriangleNodes(NodeIndex n0, NodeIndex n1, NodeIndex n2) :
   m_nodes{ n0, n1, n2 }
{
   if (n0 == n1 || n0 == n2 || n1 == n2)
   {
      std::ostringstream os;
      os << *this;
      throw MyException("TriangleNodes invalid, duplicate nodes " + os.str());
   }
}

TriangleNodes::TriangleNodes(const std::array<NodeIndex, NumNodesOnTriangle>& nodes) :
   TriangleNodes(nodes[0], nodes[1], nodes[2])
{
}

bool TriangleNodes::contains(NodeIndex node) const
{
   return node == m_nodes[0] || node == m_nodes[1] || node == m_nodes[2];
}

bool TriangleNodes::contains(const EdgeNodesDirected& edge) const
{
   return contains(edge[0]) && contains(edge[1]);
}

NodeIndex TriangleNodes::oppositeNode(const EdgeNodesDirected& edge) const
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

std::array<EdgeNodesDirected, NumEdgesOnTriangle> TriangleNodes::getEdges() const
{
   const auto& adjacencies = ReferenceShapePolygon::GetTriangle().getAdjacencies();
   const auto* adjacencyC2E = *adjacencies.getAdjacency(Topology::Corner, Topology::Edge);
   const auto& e0 = adjacencyC2E->getConnectedLowers(0);
   const auto& e1 = adjacencyC2E->getConnectedLowers(1);
   const auto& e2 = adjacencyC2E->getConnectedLowers(2);
   return {
     EdgeNodesDirected(m_nodes[e0[0]], m_nodes[e0[1]]),
     EdgeNodesDirected(m_nodes[e1[0]], m_nodes[e1[1]]),
     EdgeNodesDirected(m_nodes[e2[0]], m_nodes[e2[1]]),
   };
}