#pragma once

#include "GraphIsomorphismGraphComparers.h"
#include "GraphIsomorphismVertexComparers.h"

namespace GraphIsomorphism
{

class Comparers : public IGraphCompare, public IVertexCompare
{
  public:
    explicit Comparers(std::vector<std::unique_ptr<ICompare>> &&);

    const Graph::IGraphUs &getGraph() const override;
    std::weak_ordering compareOtherGraph(const IGraphCompare &) const override;
    std::weak_ordering compareVertexOtherGraph(Graph::Vertex, const IVertexCompare &, Graph::Vertex) const override;

  private:
    std::vector<std::unique_ptr<ICompare>> m_comparers;
    GraphComparers m_graphComparers;
    VertexComparers m_vertexComparers;
};

} // namespace GraphIsomorphism
