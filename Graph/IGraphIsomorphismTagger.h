#pragma once

#include "GraphIsomorphismDefines.h"

class UndirectedGraph;

namespace GraphIsomorphism
{

class IGraphIsomorphismTagger
{
  public:
    virtual ~IGraphIsomorphismTagger() = default;
    virtual VertexTag getTag(GraphVertex) const = 0;
    virtual const UndirectedGraph &getGraph() const = 0;
};

} // namespace GraphIsomorphism
