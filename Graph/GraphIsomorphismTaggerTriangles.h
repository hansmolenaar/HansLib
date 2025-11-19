#pragma once

#include "GraphIsomorphismDefines.h"
#include "IGraphIsomorphismTagger.h"
#include "IGraphIsomorphismVertexCompare.h"
#include "IGraphUs.h"

namespace GraphIsomorphism
{

class TaggerTriangles : public IGraphTagger, public IVertexTagger, public IVertexCompare
{
  public:
    explicit TaggerTriangles(const Graph::IGraphUs &);

    const Tag &getGraphTag() const override;
    const Tag &getVertexTag(GraphVertex) const override;
    GraphVertex getNumVertices() const override;

    bool less(GraphVertex, GraphVertex) const override;
    bool equal(GraphVertex, const IVertexCompare &, GraphVertex) const override;

    static std::vector<std::array<GraphVertex, 3>> getAllTriangles(const Graph::IGraphUs &);

  private:
    Tag m_graphTag;
    std::vector<size_t> m_countPerVertex;
};

class TaggerTrianglesFactory : public ITaggerFactory
{
  public:
    std::unique_ptr<ITagger> createTagger(const Graph::IGraphUs &) override;
};

} // namespace GraphIsomorphism
