#include "GraphUsc.h"
#include "Defines.h"
#include "MyAssert.h"
#include "UndirectedGraph.h"

using namespace Graph;
using namespace Utilities;

GraphUsc::GraphUsc(const UndirectedGraph &graph) : m_graph(graph)
{
    if (m_graph.getNumVertices() > 0)
    {
        Utilities::MyAssert(str::max(m_graph.getConnectedComponents()) == 0);
    }
}

GraphVertex GraphUsc::getNumVertices() const
{
    return m_graph.getNumVertices();
}

std::vector<GraphVertex> GraphUsc::getConnectedComponents() const
{
    return m_graph.getConnectedComponents();
}

void GraphUsc::setAdjacentVertices(GraphVertex vertex, std::vector<GraphVertex> &result) const
{
    return m_graph.setAdjacentVertices(vertex, result);
}

GraphEdge GraphUsc::getNumEdges() const
{
    return m_graph.getNumEdges();
}

GraphVertex GraphUsc::getDegree(GraphVertex vertex) const
{
    return m_graph.getDegree(vertex);
}

bool GraphUsc::areAdjacent(GraphVertex v1, GraphVertex v2) const
{
    return m_graph.areAdjacent(v1, v2);
}

GraphUsc GraphUsc::CreatePermuted(const IGraphUsc &graph, const Permutation &permut)
{
    const auto nVertices = graph.getNumVertices();
    MyAssert(nVertices == permut.getCardinality());
    UndirectedGraph result(nVertices, "Permutation of " + graph.getName());
    std::vector<GraphVertex> ngbs;
    for (GraphVertex v0 = 0; v0 < nVertices; ++v0)
    {
        const auto v1 = permut(v0);
        graph.setAdjacentVertices(v0, ngbs);
        for (auto n0 : ngbs)
        {
            const auto n1 = permut(n0);
            if (n1 > v1) // add once
            {
                result.addEdge(n1, v1);
            }
        }
    }

    return GraphUsc(result);
}

GraphUsc GraphUsc::CreatePermuted(const IGraphUsc &graph, std::initializer_list<Permutation::Entry> permut)
{
    return CreatePermuted(graph, Permutation::Create(permut));
}

std::string GraphUsc::getName() const
{
    return m_graph.getName();
}
