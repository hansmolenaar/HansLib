#pragma once

#include "GraphIsomorphismDefines.h"
#include "IGraphIsomorphismTagger.h"
#include "IGraphIsomorphismVertexCompare.h"
#include "IGraphUsc.h"

namespace GraphIsomorphism
{

class TaggerChains : public IVertexTagger, public IGraphTagger, public IVertexCompare
{
  public:
    explicit TaggerChains(const Graph::IGraphUs &);
    const Tag &getVertexTag(GraphVertex) const;
    const Tag &getGraphTag() const override;

    const Graph::IGraphUs &getGraph() const override;
    std::weak_ordering compare(GraphVertex, GraphVertex) const override;
    std::weak_ordering compareOtherGraph(GraphVertex, const IVertexCompare &, GraphVertex) const override;

  private:
    const Graph::IGraphUs &m_graph;
    Tag m_graphTag;
    std::vector<Tag> m_vertexTags;
};

class TaggerChainsFactory : public ITaggerFactory
{
  public:
    std::unique_ptr<ITagger> createTagger(const Graph::IGraphUs &) override;
};

} // namespace GraphIsomorphism
