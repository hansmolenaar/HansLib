#pragma once

#include "GraphIsomorphismDefines.h"
#include "IGraphIsomorphismTagger.h"
#include "IGraphUSC.h"

class UndirectedGraph;

namespace GraphIsomorphism
{

class TaggerChains : public IVertexTagger
{
  public:
    explicit TaggerChains(const Graph::IGraphUSC &);
    const Tag& getVertexTag(GraphVertex) const override;

  private:
    std::vector<Tag> m_tags;
};

class TaggerChainsFactory : public IVertexTaggerFactory
{
  public:
    std::unique_ptr<IVertexTagger> createVertexTagger(const Graph::IGraphUSC &) override;
};

} // namespace GraphIsomorphism
