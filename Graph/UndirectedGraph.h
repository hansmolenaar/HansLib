#pragma once

#include "IGraphUS.h"
#include "Permutation.h"

#include <string>

class UndirectedGraph : public Graph::IGraphUS
{
  public:
    explicit UndirectedGraph(GraphVertex numVertices);
    void addEdge(GraphVertex vertex1, GraphVertex vertex2);

    GraphVertex getNumVertices() const override;
    GraphEdge getNumEdges() const override;
    void setAdjacentVertices(GraphVertex vertex, std::vector<GraphVertex> &result) const override;
    bool areAdjacent(GraphVertex v1, GraphVertex v2) const override;
    GraphVertex getDegree(GraphVertex vertex) const override;

    std::vector<GraphVertex> getConnectedComponents() const;
    std::vector<GraphVertex> getDegreeSequence() const;
    std::vector<GraphVertex> getSortedDegreeSequence() const;
    std::vector<GraphVertex> getIsolatedVertices() const;
    std::string toString() const;

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
