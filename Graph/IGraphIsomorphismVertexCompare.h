#pragma once

#include "GraphIsomorphismDefines.h"

namespace GraphIsomorphism
{

class IVertexCompare
{
  public:
    ~IVertexCompare() = default;
    virtual bool less(GraphVertex, GraphVertex) const = 0;
    virtual bool equal(GraphVertex, const IVertexCompare &, GraphVertex) const = 0;
};

} // namespace GraphIsomorphism
