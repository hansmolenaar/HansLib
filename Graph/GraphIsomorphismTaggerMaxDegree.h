#pragma once

#include "GraphIsomorphismDefines.h"
#include "IGraphIsomorphismTagger.h"
#include "IGraphUsc.h"

namespace GraphIsomorphism
{

class TaggerMaxDegree : public IGraphTagger, public IVertexCompare
{
  public:
    explicit TaggerMaxDegree(const Graph::IGraphUs &);

    const Tag &getGraphTag() const override;
    TagEntry getVertexTag(Graph::GraphVertex) const;
    const Graph::IGraphUs &getGraph() const override;
    std::weak_ordering compareOtherGraph(Graph::GraphVertex, const IVertexCompare &, Graph::GraphVertex) const override;

  private:
    const Graph::IGraphUs &m_graph;
    std::map<Graph::GraphVertex, TagEntry> m_universal2id;
    Tag m_graphTag;
};

class TaggerMaxDegreeFactory : public ITaggerFactory
{
  public:
    std::unique_ptr<ITagger> createTagger(const Graph::IGraphUs &) override;
};

} // namespace GraphIsomorphism
