#pragma once

#include "IGraphUs.h"
#include "Permutation.h"

#include <string>

namespace Graph
{

class UndirectedGraph : public Graph::IGraphUs
{
  public:
    explicit UndirectedGraph(const Graph::IGraphUs &);
    explicit UndirectedGraph(GraphVertex);
    void addEdge(GraphVertex, GraphVertex);

    GraphVertex getNumVertices() const override;
    GraphEdge getNumEdges() const override;
    void setAdjacentVertices(GraphVertex vertex, std::vector<GraphVertex> &) const override;
    bool areAdjacent(GraphVertex, GraphVertex) const override;
    GraphVertex getDegree(GraphVertex) const override;
    std::vector<GraphVertex> getConnectedComponents() const override;

    std::vector<GraphVertex> getDegreeSequence() const;
    std::vector<GraphVertex> getSortedDegreeSequence() const;
    std::vector<GraphVertex> getIsolatedVertices() const;
    std::string toString() const;

    static UndirectedGraph CreatePermuted(const IGraphUs &, const Permutation &permut);
    static UndirectedGraph CreatePermuted(const IGraphUs &, std::initializer_list<Permutation::Entry>);
    static UndirectedGraph CreateComplement(const IGraphUs &);

    struct CyclesAndPaths
    {
        std::vector<std::vector<GraphVertex>> Cycles;
        std::vector<std::vector<GraphVertex>> Paths;
    };
    CyclesAndPaths SplitInCyclesAndPaths() const;

  private:
    boost::adjacency_list<boost::listS, boost::vecS, boost::undirectedS, GraphVertex, GraphEdge> m_graph;
};

} // namespace Graph
