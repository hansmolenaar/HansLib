#pragma once

#include "GraphIsomorphismDefines.h"

class UndirectedGraph;

namespace GraphIsomorphism
{

class ITagger
{
  public:
    virtual ~ITagger() = default;
    virtual VertexTag getTag(GraphVertex) const = 0;
    virtual const UndirectedGraph &getGraph() const = 0;
};

} // namespace GraphIsomorphism
