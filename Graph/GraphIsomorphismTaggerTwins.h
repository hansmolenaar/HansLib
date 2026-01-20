#pragma once

#include "GraphIsomorphismDefines.h"
#include "IGraphIsomorphismTagger.h"

namespace GraphIsomorphism
{

class TaggerTwins : public IGraphTagger, public IVertexCompare
{
  public:
    explicit TaggerTwins(const Graph::IGraphUs &);

    const Tag &getGraphTag() const override;
    const Tag &getVertexTag(Graph::Vertex) const;
    const Graph::IGraphUs &getGraph() const override;
    std::weak_ordering compareOtherGraph(Graph::Vertex, const IVertexCompare &, Graph::Vertex) const override;

  private:
    const Graph::IGraphUs &m_graph;
    std::map<Graph::Vertex, Tag> m_vertexTwinsTag;
    Tag m_graphTag;
};

class TaggerTwinsFactory : public ITaggerFactory
{
  public:
    std::unique_ptr<ITagger> createTagger(const Graph::IGraphUs &) override;
};

} // namespace GraphIsomorphism
