#pragma once

#include "GraphIsomorphismDefines.h"
#include "IGraphIsomorphismTagger.h"
#include "IGraphUsc.h"

namespace GraphIsomorphism
{

class TaggerChains : public IVertexTagger
{
  public:
    explicit TaggerChains(const Graph::IGraphUs &);
    const Tag &getVertexTag(GraphVertex) const override;
    GraphVertex getNumVertices() const override;

  private:
    std::vector<Tag> m_tags;
};

class TaggerChainsFactory : public IVertexTaggerFactory
{
  public:
    std::unique_ptr<IVertexTagger> createVertexTagger(const Graph::IGraphUs &) override;
};

} // namespace GraphIsomorphism
