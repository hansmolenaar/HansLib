#pragma once

#include "GraphIsomorphismStatus.h"
#include "IGraphUsc.h"

namespace GraphIsomorphism
{

class IGraphTaggerFactory;
class IVertexTaggerFactory;

class Construct
{
  public:
    Status actionConnected(const Graph::IGraphUsc &, const Graph::IGraphUsc &) const;
    static std::vector<IGraphTaggerFactory *> getGraphTaggerFactories();
    static std::vector<IVertexTaggerFactory *> getVertexTaggerFactories();
};

} // namespace GraphIsomorphism
