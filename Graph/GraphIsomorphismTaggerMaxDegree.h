#pragma once

#include "GraphIsomorphismDefines.h"
#include "IGraphIsomorphismTagger.h"
#include "IGraphUsc.h"

namespace GraphIsomorphism
{

class TaggerMaxDegree : public IVertexTagger, public IGraphTagger
{
  public:
    explicit TaggerMaxDegree(const Graph::IGraphUs &);

    const Tag &getGraphTag() const override;
    const Tag &getVertexTag(GraphVertex) const override;
    GraphVertex getNumVertices() const override;

  private:
    GraphVertex m_numVertices;
    std::map<GraphVertex, TagEntry> m_fullyConnected2id;
    Tag m_graphTag;
};

class TaggerMaxDegreeFactory : public ITaggerFactory
{
  public:
    std::unique_ptr<ITagger> createTagger(const Graph::IGraphUs &) override;
};

} // namespace GraphIsomorphism
