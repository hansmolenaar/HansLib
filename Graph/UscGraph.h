#pragma once

#include "IGraphUSC.h"

class UndirectedGraph;

namespace Graph
{

class UscGraph : public IGraphUSC
{
  public:
    explicit UscGraph(const UndirectedGraph& );
    GraphVertex getNumVertices() const override;
    GraphEdge getNumEdges() const override;
    void setAdjacentVertices(GraphVertex vertex, std::vector<GraphVertex> &result) const override;
    bool areAdjacent(GraphVertex v1, GraphVertex v2) const override;
    GraphVertex getDegree(GraphVertex vertex) const override;
    std::vector<GraphVertex> getConnectedComponents() const override;

    //static UndirectedGraph CreatePermuted(const IGraphUS &graph, const Permutation &permut);
  private:
    const UndirectedGraph& m_graph;
};
}
