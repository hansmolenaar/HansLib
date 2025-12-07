#include "SubGraph.h"
#include "Defines.h"
#include "MyAssert.h"
#include "UndirectedGraph.h"

using namespace Graph;
using namespace Utilities;

namespace
{

std::optional<Vertex> ToLocal(Vertex vertexInParent, const std::vector<Vertex> &verticesInParent)
{
    const auto itr = str::find(verticesInParent, vertexInParent);
    if (itr == verticesInParent.end())
    {
        return {};
    }
    auto result = std::distance(verticesInParent.begin(), itr);
    return result;
}

UndirectedGraph Generate(const IGraphUs &master, const std::vector<Vertex> &verticesInParent)
{
    const std::string name = "SubGraph of size " + std::to_string(verticesInParent.size()) + " of " + master.getName();
    const Vertex nVertices = verticesInParent.size();
    UndirectedGraph graph(nVertices, name);

    std::vector<Vertex> ngbs;
    for (Vertex v = 0; v < master.getNumVertices(); ++v)
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

SubGraph::SubGraph(const IGraphUs &master, const std::set<Vertex> &subSet)
    : m_verticesInParent(subSet.begin(), subSet.end()), m_graph(Generate(master, m_verticesInParent))
{
}

Vertex SubGraph::getNumVertices() const
{
    return m_graph.getNumVertices();
}

Edge SubGraph::getNumEdges() const
{
    return m_graph.getNumEdges();
}

void SubGraph::setAdjacentVertices(Vertex vertex, std::vector<Vertex> &result) const

{
    return m_graph.setAdjacentVertices(vertex, result);
}

bool SubGraph::areAdjacent(Vertex v1, Vertex v2) const
{
    return m_graph.areAdjacent(v1, v2);
}

Vertex SubGraph::getDegree(Vertex vertex) const
{
    return m_graph.getDegree(vertex);
}

std::vector<Vertex> SubGraph::getConnectedComponents() const
{
    return m_graph.getConnectedComponents();
}

Vertex SubGraph::getVertexInParent(Vertex v) const
{
    return m_verticesInParent.at(v);
}

std::string SubGraph::getName() const
{
    return m_graph.getName();
}
