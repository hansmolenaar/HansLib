#pragma once
#include "TopologyDefines.h"

namespace Topology
{
   using DirectedEdgeNodes = std::array<NodeIndex, NumNodesOnEdge>;
   void DirectedEdgeNodesSort(DirectedEdgeNodes& edge);
   bool DirectedEdgeNodesContains(const DirectedEdgeNodes& edge, NodeIndex node);
}