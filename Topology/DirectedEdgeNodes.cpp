#include "DirectedEdgeNodes.h"

void Topology::DirectedEdgeNodesSort(DirectedEdgeNodes& edge)
{
   if (edge[0] > edge[1]) std::swap(edge[0], edge[1]);
}

bool Topology::DirectedEdgeNodesContains(const DirectedEdgeNodes& edge, NodeIndex node)
{
   return node == edge[0] || node == edge[1];
}