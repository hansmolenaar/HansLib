#pragma once

#include "GraphIsomorphismDefines.h"
#include "IGraphIsomorphismTagger.h"

class UndirectedGraph;

namespace GraphIsomorphism
{

class TaggerChains : public ITagger
{
  public:
    explicit TaggerChains(const UndirectedGraph &);
    const UndirectedGraph &getGraph() const override;
    VertexTag getTag(GraphVertex) const override;

  private:
    const UndirectedGraph &m_graph;
    std::vector<VertexTag> m_tags;
};

class TaggerChainsFactory : public ITaggerFactory
{
  public:
    std::unique_ptr<ITagger> create(const UndirectedGraph &) override;
};


} // namespace GraphIsomorphism
