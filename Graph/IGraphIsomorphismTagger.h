#pragma once

#include "GraphIsomorphismDefines.h"

namespace Graph
{
class IGraphUS;
class IGraphUSC;
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
    virtual std::unique_ptr<IGraphTagger> createGraphTagger(const Graph::IGraphUS &) = 0;

    bool haveSameTags(const Graph::IGraphUS &, const Graph::IGraphUS &);
};

class IVertexTaggerFactory
{
  public:
    virtual ~IVertexTaggerFactory() = default;
    virtual std::unique_ptr<IVertexTagger> createVertexTagger(const Graph::IGraphUSC &) = 0;
};

} // namespace GraphIsomorphism
