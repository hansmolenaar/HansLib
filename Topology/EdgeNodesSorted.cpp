#include "EdgeNodesSorted.h"

using namespace Topology;

EdgeNodesSorted::EdgeNodesSorted(NodeIndex n0, NodeIndex n1) : m_edge{std::min(n0, n1), std::max(n0, n1)}
{
}

EdgeNodesSorted::EdgeNodesSorted(const EdgeNodesDirected &edge) : EdgeNodesSorted(edge[0], edge[1])
{
}

bool EdgeNodesSorted::contains(NodeIndex node) const
{
    return m_edge.contains(node);
}