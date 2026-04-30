#pragma once

#include "GraphIsomorphismDefines.h"
#include "IGraphIsomorphismTagger.h"
#include "IGraphUs.h"

namespace GraphIsomorphism
{

class TaggerShortestCycle : public IGraphCompare, public IVertexCompare
{
  public:
    explicit TaggerShortestCycle(const Graph::IGraphUs &);

    const Tag &getGraphTag() const;
    std::weak_ordering compareGraph(const IGraphCompare &) const override;

    const Graph::IGraphUs &getGraph() const override;
    std::weak_ordering compareVertexOtherGraph(Graph::Vertex, const IVertexCompare &, Graph::Vertex) const override;
    const VertexGrouping &getVertexGrouping() const override;

  private:
    const Graph::IGraphUs &m_graph;
    std::vector<Graph::Vertex> m_shortestCycle;
    Tag m_graphTag;
    VertexGrouping m_vertexGrouping;
};

class CompareShortestCycleFactory : public ICompareFactory
{
  public:
    std::unique_ptr<ICompare> createCompare(const Graph::IGraphUs &) const override;
};

} // namespace GraphIsomorphism
