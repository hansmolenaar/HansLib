#pragma once

#include "GraphIsomorphismDefines.h"

namespace Graph
{
class IGraphUs;
class IGraphUsc;
} // namespace Graph

namespace GraphIsomorphism
{

class IGraphTagger
{
  public:
    virtual ~IGraphTagger() = default;
    virtual const Tag &getGraphTag() const = 0;
};

class IVertexTagger
{
  public:
    virtual ~IVertexTagger() = default;
    virtual const Tag &getVertexTag(GraphVertex) const = 0;
    virtual GraphVertex getNumVertices() const = 0;
};

class IGraphTaggerFactory
{
  public:
    virtual ~IGraphTaggerFactory() = default;
    virtual std::unique_ptr<IGraphTagger> createGraphTagger(const Graph::IGraphUs &) = 0;

    bool haveSameTags(const Graph::IGraphUs &, const Graph::IGraphUs &);
};

class IVertexTaggerFactory
{
  public:
    virtual ~IVertexTaggerFactory() = default;
    virtual std::unique_ptr<IVertexTagger> createVertexTagger(const Graph::IGraphUsc &) = 0;
};

} // namespace GraphIsomorphism
