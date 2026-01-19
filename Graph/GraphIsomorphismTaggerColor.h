#pragma once

#include "GraphIsomorphismDefines.h"
#include "IGraphIsomorphismTagger.h"

namespace GraphIsomorphism
{

class TaggerColor : public IGraphTagger, public IVertexCompare
{
  public:
    explicit TaggerColor(const Graph::IGraphUs &);

    const Tag &getGraphTag() const override;
    TagEntry getVertexTag(Graph::Vertex) const;
    const Graph::IGraphUs &getGraph() const override;
    std::weak_ordering compareOtherGraph(Graph::Vertex, const IVertexCompare &, Graph::Vertex) const override;

  private:
    const Graph::IGraphUs &m_graph;
    std::vector<TagEntry> m_vertexColorTag;
    Tag m_graphTag;
};

class TaggerColorFactory : public ITaggerFactory
{
  public:
    std::unique_ptr<ITagger> createTagger(const Graph::IGraphUs &) override;
};

} // namespace GraphIsomorphism
