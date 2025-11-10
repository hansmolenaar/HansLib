#pragma once

#include "GraphIsomorphismDefines.h"
#include "IGraphIsomorphismTagger.h"
#include "IGraphUsc.h"

namespace GraphIsomorphism
{

class TaggerChains : public IVertexTagger, public IGraphTagger
{
  public:
    explicit TaggerChains(const Graph::IGraphUs &);
    const Tag &getVertexTag(GraphVertex) const override;
    GraphVertex getNumVertices() const override;
    const Tag &getGraphTag() const override;

  private:
    Tag m_graphTag;
    std::vector<Tag> m_vertexTags;
};

class TaggerChainsFactory : public ITaggerFactory
{
  public:
    std::unique_ptr<ITagger> createTagger(const Graph::IGraphUs &) override;
};

} // namespace GraphIsomorphism
