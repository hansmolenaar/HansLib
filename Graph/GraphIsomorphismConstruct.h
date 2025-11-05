#pragma once

#include "GraphIsomorphismStatus.h"
#include "IGraphUsc.h"

namespace GraphIsomorphism
{

class IGraphTaggerFactory;

class Construct
{
  public:
    Status actionConnected(const Graph::IGraphUsc &, const Graph::IGraphUsc &) const;
    static const std::vector<IGraphTaggerFactory *> &getGraphTaggerFactories();
};

} // namespace GraphIsomorphism
