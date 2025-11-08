#include "SubGraphConnected.h"
#include "Defines.h"
#include "MyAssert.h"
#include "UndirectedGraph.h"

using namespace Graph;
using namespace Utilities;

SubGraphConnected::SubGraphConnected(const IGraphUs &master, const std::set<GraphVertex> &subSet)
    : m_subGraph(master, subSet)
{
    MyAssert(m_subGraph.isConnected());
}

GraphVertex SubGraphConnected::getNumVertices() const
{
    return m_subGraph.getNumVertices();
}

GraphEdge SubGraphConnected::getNumEdges() const
{
    return m_subGraph.getNumEdges();
}

void SubGraphConnected::setAdjacentVertices(GraphVertex vertex, std::vector<GraphVertex> &result) const
{
    return m_subGraph.setAdjacentVertices(vertex, result);
}

bool SubGraphConnected::areAdjacent(GraphVertex v1, GraphVertex v2) const
{
    return m_subGraph.areAdjacent(v1, v2);
}

GraphVertex SubGraphConnected::getDegree(GraphVertex vertex) const
{
    return m_subGraph.getDegree(vertex);
}

std::vector<GraphVertex> SubGraphConnected::getConnectedComponents() const
{
    return m_subGraph.getConnectedComponents();
}

GraphVertex SubGraphConnected::getVertexInParent(GraphVertex v) const
{
    return m_subGraph.getVertexInParent(v);
}
