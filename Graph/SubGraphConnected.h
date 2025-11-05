#pragma once

#include "GraphUsc.h"

namespace Graph
{

class SubGraphConnected : public IGraphUsc
{
  public:
    SubGraphConnected(const IGraphUs &, const std::set<GraphVertex> &);
    GraphVertex getNumVertices() const override;
    GraphEdge getNumEdges() const override;
    void setAdjacentVertices(GraphVertex, std::vector<GraphVertex> &) const override;
    bool areAdjacent(GraphVertex, GraphVertex) const override;
    GraphVertex getDegree(GraphVertex) const override;
    std::vector<GraphVertex> getConnectedComponents() const override;
    GraphVertex getVertexInMaster(GraphVertex) const;

  private:
    std::vector<GraphVertex> m_verticesInMaster;
    GraphUsc m_graph;
};
} // namespace Graph
