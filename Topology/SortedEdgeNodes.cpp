#include "SortedEdgeNodes.h"

Topology::SortedEdgeNodes::SortedEdgeNodes(NodeIndex n0, NodeIndex n1) :
   m_edge{ std::min(n0, n1), std::max(n0, n1) }
{
}