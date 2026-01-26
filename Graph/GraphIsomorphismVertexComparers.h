#pragma once

#include "GraphIsomorphismGrouping.h"
#include "IGraphIsomorphismTagger.h"

namespace GraphIsomorphism
{

class VertexComparers : public IVertexCompare
{
  public:
    explicit VertexComparers(std::vector<const IVertexCompare *>);

    const Graph::IGraphUs &getGraph() const override;
    std::weak_ordering compareVertexOtherGraph(Graph::Vertex, const IVertexCompare &, Graph::Vertex) const override;
    std::weak_ordering operator<=>(const VertexComparers &) const;

    const VertexGrouping &getVertexGrouping() const;

  private:
    std::vector<const IVertexCompare *> m_vertexComparers;
    std::map<Graph::Vertex, size_t> m_vertex2group;
    VertexGrouping m_grouping;
};

} // namespace GraphIsomorphism
