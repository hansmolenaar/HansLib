#pragma once

#include "GraphIsomorphismDefines.h"

namespace Graph
{
class IGraphUs;
class IGraphUsc;
} // namespace Graph

namespace GraphIsomorphism
{

class ITagger
{
  public:
    virtual ~ITagger() = default;
};

class IGraphTagger : public ITagger
{
  public:
    virtual const Tag &getGraphTag() const = 0;
};

class IVertexTagger : public ITagger
{
  public:
    virtual const Tag &getVertexTag(GraphVertex) const = 0;
    virtual GraphVertex getNumVertices() const = 0;
};

class ITaggerFactory
{
  public:
    virtual ~ITaggerFactory() = default;
};

class IGraphTaggerFactory : public ITaggerFactory
{
  public:
    virtual std::unique_ptr<IGraphTagger> createGraphTagger(const Graph::IGraphUs &) = 0;

    bool haveSameTags(const Graph::IGraphUs &, const Graph::IGraphUs &);
};

class IVertexTaggerFactory : public ITaggerFactory
{
  public:
    virtual std::unique_ptr<IVertexTagger> createVertexTagger(const Graph::IGraphUsc &) = 0;
};

} // namespace GraphIsomorphism
