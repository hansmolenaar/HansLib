#pragma once

#include "GraphIsomorphismDefines.h"

class UndirectedGraph;

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

class ITaggeR
{
  public:
    virtual ~ITaggeR() = default;
};

class IGraphTagger : public ITaggeR
{
  public:
    virtual const Tag &getGraphTag() const = 0;
};

class IVertexTagger : public ITaggeR
{
  public:
    virtual const Tag &getVertexTag(GraphVertex) const = 0;
};

} // namespace GraphIsomorphism
