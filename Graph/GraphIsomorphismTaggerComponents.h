#pragma once

#include "GraphIsomorphismDefines.h"
#include "IGraphIsomorphismTagger.h"
#include "IGraphUS.h"

namespace GraphIsomorphism
{

class TaggerComponents : public IGraphTagger
{
  public:
    explicit TaggerComponents(const Graph::IGraphUS &);

    const Tag &getGraphTag() const override;
    bool isConnected() const;

  private:
    bool m_isConnected;
    Tag m_graphTag;
};

class TaggerComponentFactory : public IGraphTaggerFactory
{
  public:
    std::unique_ptr<IGraphTagger> createGraphTagger(const Graph::IGraphUS &) override;
};

} // namespace GraphIsomorphism
