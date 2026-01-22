#pragma once

#include "GraphIsomorphismDefines.h"
#include "IGraphIsomorphismTagger.h"
#include "IGraphUsc.h"

namespace GraphIsomorphism
{

class TaggerMaxDegree : public IGraphTagger, public IVertexCompare
{
  public:
    explicit TaggerMaxDegree(const Graph::IGraphUs &);

    const Tag &getGraphTag() const override;
    TagEntry getVertexTag(Graph::Vertex) const;
    const Graph::IGraphUs &getGraph() const override;
    std::weak_ordering compareVertexOtherGraph(Graph::Vertex, const IVertexCompare &, Graph::Vertex) const override;

  private:
    const Graph::IGraphUs &m_graph;
    std::map<Graph::Vertex, TagEntry> m_universal2id;
    Tag m_graphTag;
};

class CompareMaxDegreeFactory : public ICompareFactory
{
  public:
    std::unique_ptr<ICompare> createCompare(const Graph::IGraphUs &) override;
};

} // namespace GraphIsomorphism
