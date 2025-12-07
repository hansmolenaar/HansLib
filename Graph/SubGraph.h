#pragma once

#include "UndirectedGraph.h"

namespace Graph
{

class SubGraph : public IGraphUs
{
  public:
    SubGraph(const IGraphUs &, const std::set<GraphVertex> &);
    GraphVertex getNumVertices() const override;
    Edge getNumEdges() const override;
    void setAdjacentVertices(GraphVertex, std::vector<GraphVertex> &) const override;
    bool areAdjacent(GraphVertex, GraphVertex) const override;
    GraphVertex getDegree(GraphVertex) const override;
    std::vector<GraphVertex> getConnectedComponents() const override;
    GraphVertex getVertexInParent(GraphVertex) const;
    std::string getName() const override;

  private:
    std::vector<GraphVertex> m_verticesInParent;
    UndirectedGraph m_graph;
};
} // namespace Graph
