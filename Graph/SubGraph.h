#pragma once

#include "UndirectedGraph.h"

namespace Graph
{

class SubGraph : public IGraphUs
{
  public:
    SubGraph(const IGraphUs &, const std::set<Vertex> &);
    Vertex getNumVertices() const override;
    Edge getNumEdges() const override;
    void setAdjacentVertices(Vertex, std::vector<Vertex> &) const override;
    bool areAdjacent(Vertex, Vertex) const override;
    Vertex getDegree(Vertex) const override;
    std::vector<Vertex> getConnectedComponents() const override;
    Vertex getVertexInParent(Vertex) const;
    std::string getName() const override;

  private:
    std::vector<Vertex> m_verticesInParent;
    UndirectedGraph m_graph;
};
} // namespace Graph
