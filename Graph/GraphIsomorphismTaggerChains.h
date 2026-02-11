#pragma once

#include "GraphIsomorphismDefines.h"
#include "IGraphIsomorphismTagger.h"
#include "IGraphUsc.h"

namespace GraphIsomorphism
{

class TaggerChains : public IGraphTagger, public IVertexCompare
{
  public:
    explicit TaggerChains(const Graph::IGraphUs &);
    const Tag &getVertexTag(Graph::Vertex) const;
    const Tag &getGraphTag() const override;

    const Graph::IGraphUs &getGraph() const override;
    std::weak_ordering compareVertexOtherGraph(Graph::Vertex, const IVertexCompare &, Graph::Vertex) const override;
    const VertexGrouping &getVertexGrouping() const override;

  private:
    const Graph::IGraphUs &m_graph;
    Tag m_graphTag;
    std::vector<Tag> m_vertexTags;
    VertexGrouping m_vertexGrouping;
};

class CompareChainsFactory : public ICompareFactory
{
  public:
    std::unique_ptr<ICompare> createCompare(const Graph::IGraphUs &) const override;
};

} // namespace GraphIsomorphism
