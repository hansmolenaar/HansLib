#pragma once

#include "GraphIsomorphismDefines.h"

namespace Graph
{
class IGraphUS;
class IGraphUSC;
} // namespace Graph

namespace GraphIsomorphism
{

class ITagger
{
  public:
    virtual ~ITagger() = default;
    virtual Tag getTag(GraphVertex) const = 0;
    virtual const UndirectedGraph &getGraph() const = 0;
};

class ITaggerFactory
{
  public:
    virtual ~ITaggerFactory() = default;
    virtual std::unique_ptr<ITagger> create(const UndirectedGraph &) = 0;
};

class IGraphTagger
{
  public:
    virtual ~IGraphTagger() = default;
    virtual const Tag& getTag() const = 0;
};

class IVertexTagger
{
  public:
    virtual ~IVertexTagger() = default;
    virtual const Tag& getTag(GraphVertex) const = 0;
};

class IGraphTaggerFactory
{
  public:
    virtual ~IGraphTaggerFactory() = default;
    virtual std::unique_ptr<IGraphTagger> create(const Graph::IGraphUS &) = 0;
};

class IVertexTaggerFactory
{
  public:
    virtual ~IVertexTaggerFactory() = default;
    virtual std::unique_ptr<IVertexTagger> create(const Graph::IGraphUSC &) = 0;
};

} // namespace GraphIsomorphism
