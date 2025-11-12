#include "SubGraph.h"
#include "Defines.h"
#include "MyAssert.h"
#include "UndirectedGraph.h"

using namespace Graph;
using namespace Utilities;

namespace
{

std::optional<GraphVertex> ToLocal(GraphVertex vertexInParent, const std::vector<GraphVertex> &verticesInParent)
{
    const auto itr = str::find(verticesInParent, vertexInParent);
    if (itr == verticesInParent.end())
    {
        return {};
    }
    auto result = std::distance(verticesInParent.begin(), itr);
    return result;
}

UndirectedGraph Generate(const IGraphUs &master, const std::vector<GraphVertex> &verticesInParent)
{
    const GraphVertex nVertices = verticesInParent.size();
    UndirectedGraph graph(nVertices);

    std::vector<GraphVertex> ngbs;
    for (GraphVertex v = 0; v < master.getNumVertices(); ++v)
    {
        const auto vLocal = ToLocal(v, verticesInParent);
        if (!vLocal)
        {
            continue;
        }
        master.setAdjacentVertices(v, ngbs);
        for (auto ngb : ngbs)
        {
            const auto ngbLocal = ToLocal(ngb, verticesInParent);
            if (ngbLocal && ngb > v)
            {
                graph.addEdge(*vLocal, *ngbLocal);
            }
        }
    }
    return graph;
}

} // namespace

SubGraph::SubGraph(const IGraphUs &master, const std::set<GraphVertex> &subSet)
    : m_verticesInParent(subSet.begin(), subSet.end()), m_graph(Generate(master, m_verticesInParent))
{
}

GraphVertex SubGraph::getNumVertices() const
{
    return m_graph.getNumVertices();
}

GraphEdge SubGraph::getNumEdges() const
{
    return m_graph.getNumEdges();
}

void SubGraph::setAdjacentVertices(GraphVertex vertex, std::vector<GraphVertex> &result) const

{
    return m_graph.setAdjacentVertices(vertex, result);
}

bool SubGraph::areAdjacent(GraphVertex v1, GraphVertex v2) const
{
    return m_graph.areAdjacent(v1, v2);
}

GraphVertex SubGraph::getDegree(GraphVertex vertex) const
{
    return m_graph.getDegree(vertex);
}

std::vector<GraphVertex> SubGraph::getConnectedComponents() const
{
    return m_graph.getConnectedComponents();
}

GraphVertex SubGraph::getVertexInParent(GraphVertex v) const
{
    return m_verticesInParent.at(v);
}

std::string SubGraph::getName() const
{
    return std::string("Sub graph of ") + m_graph.getName();
}
