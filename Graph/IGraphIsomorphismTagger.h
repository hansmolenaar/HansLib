#pragma once

#include "GraphIsomorphismDefines.h"
#include "GraphIsomorphismGrouping.h"

namespace Graph
{
class IGraphUs;
class IGraphUsc;
} // namespace Graph

namespace GraphIsomorphism
{

class IVertexCompare;
class IGraphCompare;

using VertexGrouping = Grouping<Graph::Vertex>;

class IHasGraph
{
  public:
    virtual ~IHasGraph() = default;
    virtual const Graph::IGraphUs &getGraph() const = 0;
};

class ICompare : public virtual IHasGraph
{
  public:
    virtual ~ICompare() = default;

    const IVertexCompare *getVertexCompare() const;
    const IGraphCompare *getGraphCompare() const;
};

class IGraphCompare : public virtual ICompare
{
  public:
    virtual ~IGraphCompare() = default;
    virtual std::weak_ordering compareGraph(const IGraphCompare &) const = 0;
};

class IGraphCompareFactory
{
  public:
    virtual ~IGraphCompareFactory() = default;
    virtual std::unique_ptr<IGraphCompare> createGraphCompare(const Graph::IGraphUs &) const = 0;
};

class IVertexCompare : public virtual ICompare
{
  public:
    ~IVertexCompare() = default;
    virtual std::weak_ordering compareVertexOtherGraph(Graph::Vertex, const IVertexCompare &, Graph::Vertex) const = 0;
    virtual const VertexGrouping &getVertexGrouping() const = 0;
    std::weak_ordering compareGraph(const IVertexCompare &) const;
};

struct VertexLess
{
    bool operator()(Graph::Vertex v0, Graph::Vertex v1) const
    {
        return Compare.compareVertexOtherGraph(v0, Compare, v1) == std::weak_ordering::less;
    }
    const IVertexCompare &Compare;
};

class ICompareFactory
{
  public:
    virtual ~ICompareFactory() = default;
    virtual std::unique_ptr<ICompare> createCompare(const Graph::IGraphUs &) const = 0;
};

} // namespace GraphIsomorphism
