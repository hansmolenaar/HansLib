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
class IVertexTagger;

class ITagger
{
  public:
    virtual ~ITagger() = default;

    const IGraphTagger *getGraphTagger() const;
    const IVertexTagger *getVertexTagger() const;
};

class IGraphTagger : public virtual ITagger
{
  public:
    virtual const Tag &getGraphTag() const = 0;
};

class IVertexTagger : public virtual ITagger
{
  public:
    virtual const Tag &getVertexTag(GraphVertex) const = 0;
    virtual GraphVertex getNumVertices() const = 0;
    const Tag &operator()(GraphVertex v) const
    {
        return getVertexTag(v);
    };
};

class ITaggerFactory
{
  public:
    virtual ~ITaggerFactory() = default;
    virtual std::unique_ptr<ITagger> createTagger(const Graph::IGraphUs &) = 0;
};

} // namespace GraphIsomorphism
