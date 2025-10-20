#include "TetrahedronNodesOriented.h"
#include "Defines.h"
#include "FirstDuplicate.h"
#include "ITopologicalAdjacency.h"
#include "MyException.h"
#include "ReferenceShapeTetrahedron.h"
#include "StreamUtils.h"

#include <sstream>

using namespace Topology;
using namespace Utilities;

TetrahedronNodesOriented::TetrahedronNodesOriented(NodeIndex n0, NodeIndex n1, NodeIndex n2, NodeIndex n3)
    : m_nodes(orient(n0, n1, n2, n3))
{
}

TetrahedronNodesOriented::TetrahedronNodesOriented(const std::array<NodeIndex, NumCornersOnTetrahedron> &nodes)
    : m_nodes(orient(nodes))
{
}

bool TetrahedronNodesOriented::contains(NodeIndex node) const
{
    return node == m_nodes[0] || node == m_nodes[1] || node == m_nodes[2] || node == m_nodes[3];
}

size_t TetrahedronNodesOriented::find(NodeIndex node) const
{
    if (node == m_nodes[0])
        return 0;
    if (node == m_nodes[1])
        return 1;
    if (node == m_nodes[2])
        return 2;
    if (node == m_nodes[3])
        return 3;
    return NodeIndexInvalid;
}

std::array<TriangleNodesOriented, NumFacesOnTetrahedron> TetrahedronNodesOriented::getFaces() const
{
    const auto &adjacencies = ReferenceShapeTetrahedron::getInstance().getAdjacencies();
    const auto *adjacencyC2F = *adjacencies.getAdjacency(Topology::Corner, Topology::Face);
    const auto &f0 = adjacencyC2F->getConnectedLowers(0);
    const auto &f1 = adjacencyC2F->getConnectedLowers(1);
    const auto &f2 = adjacencyC2F->getConnectedLowers(2);
    const auto &f3 = adjacencyC2F->getConnectedLowers(3);
    return {TriangleNodesOriented{m_nodes[f0[0]], m_nodes[f0[1]], m_nodes[f0[2]]},
            TriangleNodesOriented{m_nodes[f1[0]], m_nodes[f1[1]], m_nodes[f1[2]]},
            TriangleNodesOriented{m_nodes[f2[0]], m_nodes[f2[1]], m_nodes[f2[2]]},
            TriangleNodesOriented{m_nodes[f3[0]], m_nodes[f3[1]], m_nodes[f3[2]]}};
}

std::array<EdgeNodesSorted, NumEdgesOnTetrahedron> TetrahedronNodesOriented::getEdges() const
{
    const auto &adjacencies = ReferenceShapeTetrahedron::getInstance().getAdjacencies();
    const auto *adjacencyC2E = *adjacencies.getAdjacency(Topology::Corner, Topology::Edge);
    const auto &e0 = adjacencyC2E->getConnectedLowers(0);
    const auto &e1 = adjacencyC2E->getConnectedLowers(1);
    const auto &e2 = adjacencyC2E->getConnectedLowers(2);
    const auto &e3 = adjacencyC2E->getConnectedLowers(3);
    const auto &e4 = adjacencyC2E->getConnectedLowers(4);
    const auto &e5 = adjacencyC2E->getConnectedLowers(5);
    return {EdgeNodesSorted{m_nodes[e0[0]], m_nodes[e0[1]]}, EdgeNodesSorted{m_nodes[e1[0]], m_nodes[e1[1]]},
            EdgeNodesSorted{m_nodes[e2[0]], m_nodes[e2[1]]}, EdgeNodesSorted{m_nodes[e3[0]], m_nodes[e3[1]]},
            EdgeNodesSorted{m_nodes[e4[0]], m_nodes[e4[1]]}, EdgeNodesSorted{m_nodes[e5[0]], m_nodes[e5[1]]}};
}

std::array<NodeIndex, NumCornersOnTetrahedron> TetrahedronNodesOriented::orient(NodeIndex n0, NodeIndex n1,
                                                                                NodeIndex n2, NodeIndex n3)
{
    return orient({n0, n1, n2, n3});
}

std::array<NodeIndex, NumCornersOnTetrahedron> TetrahedronNodesOriented::orient(
    std::array<NodeIndex, NumCornersOnTetrahedron> result)
{
    throwOnDuplicate(result);

    int numSwaps = 0;
    auto mm = str::minmax_element(result);
    auto tmp = *mm.min;
    if (mm.min != result.begin())
    {
        // Keep track of the max-element if we are about to move it
        if (mm.max == result.begin())
        {
            mm.max = mm.min;
        }
        numSwaps += 1;
        std::swap(*mm.min, result[0]);
    }
    if (mm.max != result.begin() + 3)
    {
        numSwaps += 1;
        std::swap(*mm.max, result[3]);
    }
    // Extra swap to needed to preserve the orientation?
    if (numSwaps == 1)
    {
        std::swap(result[1], result[2]);
    }

    return result;
}