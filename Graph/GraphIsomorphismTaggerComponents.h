#pragma once

#include "GraphIsomorphismDefines.h"
#include "IGraphIsomorphismTagger.h"
#include "IGraphUs.h"

namespace GraphIsomorphism
{

class TaggerComponents : public IGraphTagger
{
  public:
    explicit TaggerComponents(const Graph::IGraphUs &);

    const Graph::IGraphUs &getGraph() const override;
    const Tag &getGraphTag() const override;
    bool isConnected() const;

  private:
    const Graph::IGraphUs &m_graph;
    bool m_isConnected;
    Tag m_graphTag;
};

class TaggerComponentFactory : public ITaggerFactory
{
  public:
    std::unique_ptr<ITagger> createTagger(const Graph::IGraphUs &) override;
};

} // namespace GraphIsomorphism
