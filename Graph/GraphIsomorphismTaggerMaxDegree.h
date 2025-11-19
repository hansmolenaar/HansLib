#pragma once

#include "GraphIsomorphismDefines.h"
#include "IGraphIsomorphismTagger.h"
#include "IGraphIsomorphismVertexCompare.h"
#include "IGraphUsc.h"

namespace GraphIsomorphism
{

class TaggerMaxDegree : public IVertexTagger, public IGraphTagger, public IVertexCompare
{
  public:
    explicit TaggerMaxDegree(const Graph::IGraphUs &);

    const Tag &getGraphTag() const override;
    const Tag &getVertexTag(GraphVertex) const override;
    GraphVertex getNumVertices() const override;

    const Graph::IGraphUs &getGraph() const override;
    bool less(GraphVertex, GraphVertex) const override;
    bool less(GraphVertex, const IVertexCompare &, GraphVertex) const override;

  private:
    const Graph::IGraphUs &m_graph;
    std::map<GraphVertex, TagEntry> m_fullyConnected2id;
    Tag m_graphTag;
};

class TaggerMaxDegreeFactory : public ITaggerFactory
{
  public:
    std::unique_ptr<ITagger> createTagger(const Graph::IGraphUs &) override;
};

} // namespace GraphIsomorphism
