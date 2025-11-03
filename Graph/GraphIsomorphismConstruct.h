#pragma once

#include "GraphIsomorphismStatus.h"
#include "IGraphUSC.h"

namespace GraphIsomorphism
{

class Construct
{
  public:
    Status actionConnected(const Graph::IGraphUSC &, const Graph::IGraphUSC &) const;
};

} // namespace GraphIsomorphism
