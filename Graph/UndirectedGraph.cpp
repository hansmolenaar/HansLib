#include "UndirectedGraph.h"

UndirectedGraph::UndirectedGraph(GraphVertex numVertices) : m_graph(numVertices)
{
}

void UndirectedGraph::addEdge(GraphVertex vertex1, GraphVertex vertex2)
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

void UndirectedGraph::setAdjacentVertices(GraphVertex vertex, std::vector<GraphVertex>& result) const
{
   result.clear();
   const auto neighbours = boost::adjacent_vertices(vertex, m_graph);
   for (auto vd : make_iterator_range(neighbours))
   {
      result.push_back(vd);
   }
}

GraphEdge UndirectedGraph::getNumEdges() const
{
   return boost::num_edges(m_graph);
}