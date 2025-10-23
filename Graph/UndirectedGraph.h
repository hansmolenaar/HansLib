#pragma once

#include "GraphDefines.h"
#include "Permutation.h"

class UndirectedGraph
{
  public:
    explicit UndirectedGraph(GraphVertex numVertices);
    void addEdge(GraphVertex vertex1, GraphVertex vertex2);
    GraphVertex getNumVertices() const;
    GraphEdge getNumEdges() const;
    std::vector<GraphVertex> getConnectedComponents() const;
    void setAdjacentVertices(GraphVertex vertex, std::vector<GraphVertex> &result) const;
    bool areAdjacent(GraphVertex v1, GraphVertex v2) const;
    GraphVertex getDegree(GraphVertex vertex) const;
    std::vector<GraphVertex> getDegreeSequence() const;
    std::vector<GraphVertex> getSortedDegreeSequence() const;
    std::vector<GraphVertex> getIsolatedVertices() const;

    static UndirectedGraph CreatePermuted(const UndirectedGraph &graph, const Permutation &permut);

    struct CyclesAndPaths
    {
        std::vector<std::vector<GraphVertex>> Cycles;
        std::vector<std::vector<GraphVertex>> Paths;
    };
    CyclesAndPaths SplitInCyclesAndPaths() const;

  private:
    boost::adjacency_list<boost::listS, boost::vecS, boost::undirectedS, GraphVertex, GraphEdge> m_graph;
};
