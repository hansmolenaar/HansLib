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
};

} // namespace GraphIsomorphism
