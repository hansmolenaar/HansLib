#include "SubGraphConnected.h"
#include "Defines.h"
#include "MyAssert.h"
#include "UndirectedGraph.h"

using namespace Graph;
using namespace Utilities;

namespace
{

std::optional<GraphVertex> ToLocal(GraphVertex vertexInMaster, const std::vector<GraphVertex> &verticesInMaster)
{
    const auto itr = str::find(verticesInMaster, vertexInMaster);
    if (itr == verticesInMaster.end())
    {
        return {};
    }
    auto result = std::distance(verticesInMaster.begin(), itr);
    return result;
}

UndirectedGraph Generate(const IGraphUs &master, const std::vector<GraphVertex> &verticesInMaster)
{
    const GraphVertex nVertices = verticesInMaster.size();
    UndirectedGraph graph(nVertices);

    std::vector<GraphVertex> ngbs;
    for (GraphVertex v = 0; v < master.getNumVertices(); ++v)
    {
        const auto vLocal = ToLocal(v, verticesInMaster);
        if (!vLocal)
        {
            continue;
        }
        master.setAdjacentVertices(v, ngbs);
        for (auto ngb : ngbs)
        {
            const auto ngbLocal = ToLocal(ngb, verticesInMaster);
            if (ngbLocal && ngb > v)
            {
                graph.addEdge(*vLocal, *ngbLocal);
            }
        }
    }
    return graph;
}

} // namespace

SubGraphConnected::SubGraphConnected(const IGraphUs &master, const std::set<GraphVertex> &subSet)
    : m_verticesInMaster(subSet.begin(), subSet.end()), m_graph(Generate(master, m_verticesInMaster))
{
}

GraphVertex SubGraphConnected::getNumVertices() const
{
    return m_graph.getNumVertices();
}

GraphEdge SubGraphConnected::getNumEdges() const
{
    return m_graph.getNumEdges();
}

void SubGraphConnected::setAdjacentVertices(GraphVertex vertex, std::vector<GraphVertex> &result) const

{
    return m_graph.setAdjacentVertices(vertex, result);
}

bool SubGraphConnected::areAdjacent(GraphVertex v1, GraphVertex v2) const
{
    return m_graph.areAdjacent(v1, v2);
}

GraphVertex SubGraphConnected::getDegree(GraphVertex vertex) const
{
    return m_graph.getDegree(vertex);
}

std::vector<GraphVertex> SubGraphConnected::getConnectedComponents() const
{
    return m_graph.getConnectedComponents();
}

GraphVertex SubGraphConnected::getVertexInMaster(GraphVertex v) const
{
    return m_verticesInMaster.at(v);
}
