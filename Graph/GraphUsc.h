#pragma once

#include "IGraphUSC.h"
#include "UndirectedGraph.h"

class UndirectedGraph;

namespace Graph
{

class UscGraph : public IGraphUSC
{
  public:
    explicit UscGraph(const UndirectedGraph &);
    GraphVertex getNumVertices() const override;
    GraphEdge getNumEdges() const override;
    void setAdjacentVertices(GraphVertex vertex, std::vector<GraphVertex> &result) const override;
    bool areAdjacent(GraphVertex v1, GraphVertex v2) const override;
    GraphVertex getDegree(GraphVertex vertex) const override;
    std::vector<GraphVertex> getConnectedComponents() const override;

    static UscGraph CreatePermuted(const IGraphUSC &, const Permutation &);

  private:
    UndirectedGraph m_graph; // TODO avoid copying?
};
} // namespace Graph
