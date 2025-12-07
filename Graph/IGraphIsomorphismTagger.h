#pragma once

#include "GraphIsomorphismDefines.h"

namespace Graph
{
class IGraphUs;
class IGraphUsc;
} // namespace Graph

namespace GraphIsomorphism
{

class IGraphTagger;
class IVertexCompare;

class ITagger
{
  public:
    virtual ~ITagger() = default;

    const IGraphTagger *getGraphTagger() const;
    const IVertexCompare *getVertexCompare() const;
};

class IGraphTagger : public virtual ITagger
{
  public:
    virtual const Tag &getGraphTag() const = 0;
};

class IVertexCompare : public virtual ITagger
{
  public:
    ~IVertexCompare() = default;
    virtual const Graph::IGraphUs &getGraph() const = 0;
    virtual std::weak_ordering compareOtherGraph(Graph::GraphVertex, const IVertexCompare &, Graph::GraphVertex) const = 0;

    std::weak_ordering compare(Graph::GraphVertex, Graph::GraphVertex) const;
};

struct VertexLess
{
    bool operator()(Graph::GraphVertex v0, Graph::GraphVertex v1) const
    {
        return Compare.compare(v0, v1) < 0;
    }
    const IVertexCompare &Compare;
};

class ITaggerFactory
{
  public:
    virtual ~ITaggerFactory() = default;
    virtual std::unique_ptr<ITagger> createTagger(const Graph::IGraphUs &) = 0;
};

} // namespace GraphIsomorphism
