#include "UndirectedGraph.h"

UndirectedGraph::UndirectedGraph(GraphVertex numVertices) : m_graph(numVertices)
{
}

void UndirectedGraph::addEdge(UGraphVertex vertex1, UGraphVertex vertex2)
{
   boost::add_edge(vertex1, vertex2, m_graph);
}

GraphVertex UndirectedGraph::getNumVertices() const
{
   return boost::num_vertices(m_graph);
}

std::vector<GraphVertex> UndirectedGraph::getConnectedComponents() const
{
   std::vector<GraphVertex> result(getNumVertices());
   boost::connected_components(m_graph, result.data());
   return result;
}