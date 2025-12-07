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
    const Tag &getVertexTag(Graph::GraphVertex) const;
    const Tag &getGraphTag() const override;

    const Graph::IGraphUs &getGraph() const override;
    std::weak_ordering compareOtherGraph(Graph::GraphVertex, const IVertexCompare &, Graph::GraphVertex) const override;

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
