#include "UscGraph.h"
#include "Defines.h"
#include "MyAssert.h"
#include "UndirectedGraph.h"

using namespace Graph;

UscGraph::UscGraph(const UndirectedGraph& graph) : m_graph(graph)
{
if (m_graph.getNumVertices() > 0)
{
 Utilities::MyAssert(str::max(m_graph.getConnectedComponents()) == 0);
}
}

GraphVertex UscGraph::getNumVertices() const
{
    return m_graph.getNumVertices();
}

std::vector<GraphVertex> UscGraph::getConnectedComponents() const
{
    return m_graph.getConnectedComponents();
}

void UscGraph::setAdjacentVertices(GraphVertex vertex, std::vector<GraphVertex> &result) const
{
   return m_graph.setAdjacentVertices(vertex, result);
}

GraphEdge UscGraph::getNumEdges() const
{
    return m_graph.getNumEdges();
}

GraphVertex UscGraph::getDegree(GraphVertex vertex) const
{
return m_graph.getDegree(vertex);
}

bool UscGraph::areAdjacent(GraphVertex v1, GraphVertex v2) const
{
return m_graph.areAdjacent(v1, v2);
}

#if false
UscGraph UscGraph::CreatePermuted(const IGraphUS &graph, const Permutation &permut)
{
    const auto nVertices = graph.getNumVertices();
    MyAssert(nVertices == permut.getCardinality());
    UndirectedGraph result(nVertices);
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

    return result;
}
#endif
