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

class IGraphTagger;
class IVertexCompare;
class ICharacteristicsCompare;

using VertexGrouping = Grouping<Graph::Vertex>;

class ICompare
{
  public:
    virtual ~ICompare() = default;
    virtual const Graph::IGraphUs &getGraph() const = 0;

    const ICharacteristicsCompare *getCharacteristicsCompare() const;
    const IGraphTagger *getGraphTagger() const;
    const IVertexCompare *getVertexCompare() const;
};

class ICharacteristicsCompare : public virtual ICompare
{
  public:
    virtual ~ICharacteristicsCompare() = default;
    virtual std::weak_ordering compareCharacteristics(const ICharacteristicsCompare &) const = 0;
};

class IGraphTagger : public virtual ICharacteristicsCompare
{
  public:
    virtual const Tag &getGraphTag() const = 0;
    std::weak_ordering compareCharacteristics(const ICharacteristicsCompare &gc) const override;
};

class IVertexCompare : public virtual ICompare
{
  public:
    ~IVertexCompare() = default;
    virtual std::weak_ordering compareVertexOtherGraph(Graph::Vertex, const IVertexCompare &, Graph::Vertex) const = 0;

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

class ICompareFactory
{
  public:
    virtual ~ICompareFactory() = default;
    virtual std::unique_ptr<ICompare> createCompare(const Graph::IGraphUs &) = 0;
};

} // namespace GraphIsomorphism
