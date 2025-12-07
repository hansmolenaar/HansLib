#pragma once

#include "IGraphUsc.h"
#include "UndirectedGraph.h"

namespace Graph
{

class GraphUsc : public IGraphUsc
{
  public:
    explicit GraphUsc(const UndirectedGraph &);
    Vertex getNumVertices() const override;
    Edge getNumEdges() const override;
    void setAdjacentVertices(Vertex vertex, std::vector<Vertex> &result) const override;
    bool areAdjacent(Vertex v1, Vertex v2) const override;
    Vertex getDegree(Vertex vertex) const override;
    std::vector<Vertex> getConnectedComponents() const override;
    std::string getName() const override;

    static GraphUsc CreatePermuted(const IGraphUsc &, const Permutation &);
    static GraphUsc CreatePermuted(const IGraphUsc &, std::initializer_list<Permutation::Entry>);

  private:
    UndirectedGraph m_graph; // TODO avoid copying?
};
} // namespace Graph
