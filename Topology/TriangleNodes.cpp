#include "MyException.h"
#include "TriangleNodes.h"

bool Topology::TriangleNodesNodesContainsNode(const TriangleNodes& triangle, NodeIndex node)
{
   return node == triangle[0] || node == triangle[1] || node == triangle[2];
}

bool Topology::TriangleNodesNodesContainsEdge(const TriangleNodes& triangle, const DirectedEdgeNodes& edge)
{
   return TriangleNodesNodesContainsNode(triangle, edge[0]) && TriangleNodesNodesContainsNode(triangle, edge[1]);
}

Topology::NodeIndex Topology::TriangleNodesNodesOppositeNode(const TriangleNodes& triangle, const DirectedEdgeNodes& edge)
{
   if (!edge.contains(triangle[0]))
   {
      if (!edge.contains(triangle[1]) || !edge.contains(triangle[2]))
      {
         throw MyException("Triangle does not contain edge");
      }
      return  triangle[0];
   }
   else if (!edge.contains(triangle[1]))
   {

      if (!edge.contains(triangle[2]))
      {
         throw MyException("Triangle does not contain edge");
      }
      return triangle[1];
   }
   else if (!edge.contains(triangle[2]))
   {
      return  triangle[2];
   }

   throw MyException("TriangleNodesNodesOppositeNode should not get here");
}