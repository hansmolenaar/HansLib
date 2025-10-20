
#include "TriangleNodesOriented.h"

using namespace Topology;

namespace
{
std::array<NodeIndex, NumCornersOnTriangle> order(NodeIndex n0, NodeIndex n1, NodeIndex n2)
{
    if (n0 < n1 && n0 < n2)
        return {n0, n1, n2};
    if (n1 < n0 && n1 < n2)
        return {n1, n2, n0};
    if (n2 < n0 && n2 < n1)
        return {n2, n0, n1};
    throw MyException("order should not get here");
}
} // namespace

TriangleNodesOriented::TriangleNodesOriented(NodeIndex n0, NodeIndex n1, NodeIndex n2) : m_triangle(order(n0, n1, n2))
{
}

// returns std::numeric_limtis<size_t>::max() if not found
size_t TriangleNodesOriented::find(NodeIndex node) const
{
    if (node == m_triangle[0])
        return 0;
    if (node == m_triangle[1])
        return 1;
    if (node == m_triangle[2])
        return 2;
    return std::numeric_limits<size_t>::max();
}

std::array<EdgeNodesDirected, NumEdgesOnTriangle> TriangleNodesOriented::getEdges() const
{
    return {EdgeNodesDirected(m_triangle[0], m_triangle[1]), EdgeNodesDirected(m_triangle[1], m_triangle[2]),
            EdgeNodesDirected(m_triangle[2], m_triangle[0])};
}

bool TriangleNodesOriented::containsDirectedEdge(const EdgeNodesDirected &edge) const
{
    return str::any_of(getEdges(), [&edge](const auto &e) { return edge == e; });
}