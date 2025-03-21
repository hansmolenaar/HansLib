#pragma once
#include "DirectedEdgeNodes.h"
#include "TopologyDefines.h"

namespace Topology
{
   using TriangleNodes = std::array<NodeIndex, NumNodesOnTriangle>;

   bool TriangleNodesNodesContainsNode(const TriangleNodes& triangle, NodeIndex node);
   bool TriangleNodesNodesContainsEdge(const TriangleNodes& triangle, const DirectedEdgeNodes& edge);
   NodeIndex TriangleNodesNodesOppositeNode(const TriangleNodes& triangle, const DirectedEdgeNodes& edge);
}