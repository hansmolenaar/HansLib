#pragma once

#include "GraphIsomorphismDefines.h"
#include "IGraphIsomorphismTagger.h"

namespace GraphIsomorphism
{

class TaggerColor : public ICharacteristicsCompare, public IVertexCompare
{
  public:
    explicit TaggerColor(const Graph::IGraphUs &);

    const Tag &getGraphTag() const;
    std::weak_ordering compareCharacteristics(const ICharacteristicsCompare &) const override;

    TagEntry getVertexTag(Graph::Vertex) const;
    const Graph::IGraphUs &getGraph() const override;
    std::weak_ordering compareVertexOtherGraph(Graph::Vertex, const IVertexCompare &, Graph::Vertex) const override;
    const VertexGrouping &getVertexGrouping() const override;

  private:
    const Graph::IGraphUs &m_graph;
    std::vector<TagEntry> m_vertexColorTag;
    Tag m_graphTag;
    VertexGrouping m_vertexGrouping;
};

class CompareColorFactory : public ICompareFactory
{
  public:
    std::unique_ptr<ICompare> createCompare(const Graph::IGraphUs &) const override;
};

} // namespace GraphIsomorphism
