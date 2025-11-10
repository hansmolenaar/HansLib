#pragma once

#include "GraphIsomorphismStatus.h"
#include "IGraphIsomorphismTagger.h"
#include "IGraphUsc.h"

namespace GraphIsomorphism
{

class Construct
{
  public:
    Status actionConnected(const Graph::IGraphUsc &, const Graph::IGraphUsc &) const;
    static std::vector<ITaggerFactory *> getTaggerFactories();
    static std::vector<std::unique_ptr<ITagger>> getAllTaggers(const Graph::IGraphUs &);
    static std::vector<const IGraphTagger *> selectGraphTaggers(const std::vector<std::unique_ptr<ITagger>> &);
    static std::vector<const IVertexTagger *> selectVertexTaggers(const std::vector<std::unique_ptr<ITagger>> &);
};

} // namespace GraphIsomorphism
