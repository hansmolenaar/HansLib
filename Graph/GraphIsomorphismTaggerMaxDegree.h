#pragma once

#include "GraphIsomorphismDefines.h"
#include "IGraphIsomorphismTagger.h"
#include "IGraphUsc.h"

namespace GraphIsomorphism
{

class TaggerMaxDegree : public ICharacteristicsCompare, public IVertexCompare
{
  public:
    explicit TaggerMaxDegree(const Graph::IGraphUs &);

    std::weak_ordering compareCharacteristics(const ICharacteristicsCompare &) const override;
    const Tag &getGraphTag() const;
    TagEntry getVertexTag(Graph::Vertex) const;
    const Graph::IGraphUs &getGraph() const override;
    std::weak_ordering compareVertexOtherGraph(Graph::Vertex, const IVertexCompare &, Graph::Vertex) const override;
    const VertexGrouping &getVertexGrouping() const override;

  private:
    const Graph::IGraphUs &m_graph;
    std::map<Graph::Vertex, TagEntry> m_universal2id;
    Tag m_graphTag;
    VertexGrouping m_vertexGrouping;
};

class CompareMaxDegreeFactory : public ICompareFactory
{
  public:
    std::unique_ptr<ICompare> createCompare(const Graph::IGraphUs &) const override;
};

} // namespace GraphIsomorphism
