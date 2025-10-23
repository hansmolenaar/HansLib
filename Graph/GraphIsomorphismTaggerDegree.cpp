#include "GraphIsomorphismTaggerDegree.h"
#include "UndirectedGraph.h"

using namespace GraphIsomorphism;

GraphIsomorphism::TaggerDegree::TaggerDegree(const UndirectedGraph &graph) : m_graph(graph)
{
}

const UndirectedGraph &TaggerDegree::getGraph() const
{
    return m_graph;
}

VertexTag TaggerDegree::getTag(GraphVertex v) const
{
    return {static_cast<int>(m_graph.getDegree(v))};
}
