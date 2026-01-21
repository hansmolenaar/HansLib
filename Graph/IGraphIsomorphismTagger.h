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
class IGraphCompare;

class ICompare
{
  public:
    virtual ~ICompare() = default;
    virtual const Graph::IGraphUs &getGraph() const = 0;

    const IGraphCompare *getGraphCompare() const;
    const IVertexCompare *getVertexCompare() const;
};

class IGraphCompare : public virtual ICompare
{
  public:
    virtual ~IGraphCompare() = default;
    virtual std::weak_ordering compareOtherGraph(const IGraphCompare&) const = 0;
};

class ITagger : public ICompare
{
  public:
    virtual ~ITagger() = default;

    const IGraphTagger *getGraphTagger() const;
    const IVertexCompare *getVertexCompare() const;
};

class IGraphTagger : public virtual ITagger, public IGraphCompare
{
  public:
    virtual const Tag &getGraphTag() const = 0;
    std::weak_ordering compareOtherGraph(const IGraphCompare& gc) const override {return compareOtherGraph(gc);};
};

class IVertexCompare : public virtual ITagger
{
  public:
    ~IVertexCompare() = default;
    virtual std::weak_ordering compareOtherGraph(Graph::Vertex, const IVertexCompare &, Graph::Vertex) const = 0;

    std::weak_ordering compare(Graph::Vertex, Graph::Vertex) const;
};

struct VertexLess
{
    bool operator()(Graph::Vertex v0, Graph::Vertex v1) const
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

class ICompareFactory
{
  public:
    virtual ~ICompareFactory() = default;
    virtual std::unique_ptr<ICompare> createCompare(const Graph::IGraphUs &) = 0;
};

} // namespace GraphIsomorphism
