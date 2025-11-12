#pragma once

#include "IGraphUsc.h"
#include "UndirectedGraph.h"

namespace Graph
{

class GraphUsc : public IGraphUsc
{
  public:
    explicit GraphUsc(const UndirectedGraph &);
    GraphVertex getNumVertices() const override;
    GraphEdge getNumEdges() const override;
    void setAdjacentVertices(GraphVertex vertex, std::vector<GraphVertex> &result) const override;
    bool areAdjacent(GraphVertex v1, GraphVertex v2) const override;
    GraphVertex getDegree(GraphVertex vertex) const override;
    std::vector<GraphVertex> getConnectedComponents() const override;
    std::string getName() const override;

    static GraphUsc CreatePermuted(const IGraphUsc &, const Permutation &);
    static GraphUsc CreatePermuted(const IGraphUsc &, std::initializer_list<Permutation::Entry>);

  private:
    UndirectedGraph m_graph; // TODO avoid copying?
};
} // namespace Graph
