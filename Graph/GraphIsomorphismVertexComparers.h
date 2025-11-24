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
    std::weak_ordering compareOtherGraph(GraphVertex, const IVertexCompare &, GraphVertex) const override;
    std::weak_ordering operator<=>(const VertexComparers &) const;

  private:
    std::vector<const IVertexCompare *> m_vertexComparers;
    std::map<GraphVertex, size_t> m_vertex2group;
    Grouping<GraphVertex> m_grouping;
};

} // namespace GraphIsomorphism
