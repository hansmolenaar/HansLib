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

class ITaggeR
{
  public:
    virtual ~ITaggeR() = default;
    virtual bool isGraphTagger() const = 0;
    virtual const Tag &getGraphTag() const = 0; // UB if not a graph tagger

    virtual bool isVertexTagger() const = 0;
    virtual const Tag &getVertexTag(GraphVertex) const = 0; // UB if not a vertex tagger
};

class ITaggerUSFactory
{
  public:
    virtual ~ITaggerUSFactory() = default;
    virtual std::unique_ptr<ITaggeR> create(const Graph::IGraphUS &) = 0;
};

class ITaggerUSCFactory
{
  public:
    virtual ~ITaggerUSCFactory() = default;
    virtual std::unique_ptr<ITaggeR> create(const Graph::IGraphUSC &) = 0;
};

} // namespace GraphIsomorphism
