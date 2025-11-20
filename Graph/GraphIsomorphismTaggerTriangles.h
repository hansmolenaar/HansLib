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

    const Graph::IGraphUs &getGraph() const override;
    std::weak_ordering compare(GraphVertex, GraphVertex) const override;
    std::weak_ordering compareOtherGraph(GraphVertex, const IVertexCompare &, GraphVertex) const override;

    static std::vector<std::array<GraphVertex, 3>> getAllTriangles(const Graph::IGraphUs &);

  private:
    const Graph::IGraphUs &m_graph;
    Tag m_graphTag;
    std::vector<size_t> m_countPerVertex;
};

class TaggerTrianglesFactory : public ITaggerFactory
{
  public:
    std::unique_ptr<ITagger> createTagger(const Graph::IGraphUs &) override;
};

} // namespace GraphIsomorphism
