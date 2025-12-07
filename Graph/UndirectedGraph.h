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
    explicit UndirectedGraph(Vertex, std::string = "");
    void addEdge(Vertex, Vertex);

    Vertex getNumVertices() const override;
    Edge getNumEdges() const override;
    void setAdjacentVertices(Vertex vertex, std::vector<Vertex> &) const override;
    bool areAdjacent(Vertex, Vertex) const override;
    Vertex getDegree(Vertex) const override;
    std::vector<Vertex> getConnectedComponents() const override;

    std::string getName() const override;

    std::vector<Vertex> getDegreeSequence() const;
    std::vector<Vertex> getSortedDegreeSequence() const;
    std::vector<Vertex> getIsolatedVertices() const;
    std::string toString() const;

    static UndirectedGraph CreatePermuted(const IGraphUs &, const Permutation &permut);
    static UndirectedGraph CreatePermuted(const IGraphUs &, std::initializer_list<Permutation::Entry>);
    static UndirectedGraph CreateComplement(const IGraphUs &);

    struct CyclesAndPaths
    {
        std::vector<std::vector<Vertex>> Cycles;
        std::vector<std::vector<Vertex>> Paths;
    };
    CyclesAndPaths SplitInCyclesAndPaths() const;

  private:
    boost::adjacency_list<boost::listS, boost::vecS, boost::undirectedS, Vertex, Edge> m_graph;
    std::string m_name;
};

} // namespace Graph
