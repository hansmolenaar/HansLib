#pragma once

#include "GraphIsomorphismGraphComparers.h"
#include "GraphIsomorphismVertexComparers.h"

namespace GraphIsomorphism
{

class Comparers : public IGraphCompare, public IVertexCompare
{
  public:
    explicit Comparers(std::vector<std::unique_ptr<ICompare>> &&);
    explicit Comparers(const Graph::IGraphUs &);

    const Graph::IGraphUs &getGraph() const override;
    std::weak_ordering compareOtherGraph(const IGraphCompare &) const override;
    std::weak_ordering compareVertexOtherGraph(Graph::Vertex, const IVertexCompare &, Graph::Vertex) const override;
    const Grouping<Graph::Vertex> &getVertexGrouping() const;

    std::weak_ordering operator<=>(const Comparers &) const;
    bool operator==(const Comparers &) const;

  private:
    std::vector<std::unique_ptr<ICompare>> m_comparers;
    GraphComparers m_graphComparers;
    VertexComparers m_vertexComparers;
};

} // namespace GraphIsomorphism
