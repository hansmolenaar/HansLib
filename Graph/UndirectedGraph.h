#pragma once

#include "GraphDefines.h"


class UndirectedGraph
{
public:
   explicit UndirectedGraph(GraphVertex numVertices);
   void addEdge(GraphVertex vertex1, GraphVertex vertex2);
   GraphVertex getNumVertices() const;
   GraphEdge getNumEdges() const;
   std::vector<GraphVertex> getConnectedComponents() const;
   void setAdjacentVertices(GraphVertex vertex, std::vector<GraphVertex>& result) const;
   GraphVertex getDegree(GraphVertex vertex) const;
   std::vector<GraphVertex> getDegreeSequence() const;

private:
   boost::adjacency_list<boost::listS, boost::vecS, boost::undirectedS, GraphVertex, GraphEdge > m_graph;
};
