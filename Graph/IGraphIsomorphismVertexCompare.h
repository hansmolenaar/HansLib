#pragma once

#include "GraphIsomorphismDefines.h"
#include "IGraphIsomorphismTagger.h"

namespace Graph
{
class IGraphUs;
}

namespace GraphIsomorphism
{

class IVertexCompare : public virtual ITagger
{
  public:
    ~IVertexCompare() = default;
    virtual const Graph::IGraphUs &getGraph() const = 0;
    virtual std::weak_ordering compare(GraphVertex, GraphVertex) const = 0;
    virtual std::weak_ordering compareOtherGraph(GraphVertex, const IVertexCompare &, GraphVertex) const = 0;
};

struct VertexLess
{
    bool operator()(GraphVertex v0, GraphVertex v1) const
    {
        return Compare.compare(v0, v1) < 0;
    }
    const IVertexCompare &Compare;
};

} // namespace GraphIsomorphism
