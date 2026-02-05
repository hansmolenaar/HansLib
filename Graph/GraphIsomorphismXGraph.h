#pragma once

#include "GraphIsomorphismComparers.h"
#include "GraphIsomorphismGrouping.h"
#include "UndirectedGraph.h"

namespace GraphIsomorphism
{

class XGraph : public IGraphCompare
{
  public:
    explicit XGraph(const Graph::IGraphUs &);

    const Graph::IGraphUs &getGraph() const override;
    std::weak_ordering compareGraph(const IGraphCompare &other) const override;

    const VertexGrouping &getVertexGrouping() const override;
    const Graph::IGraphUs &getGraphComplement() const;

  private:
    const Comparers &getComparersLineGraph() const;
    Graph::UndirectedGraph m_complement;
    std::shared_ptr<Comparers> m_comparers;
    std::shared_ptr<Comparers> m_comparesComplement;
    mutable std::unique_ptr<Graph::UndirectedGraph> m_lineGraph;
    mutable std::unique_ptr<Comparers> m_comparesLineGraph;
    VertexGrouping m_grouping;
};

class XGraphFactory : public IGraphCompareFactory
{
    std::unique_ptr<IGraphCompare> createGraphCompare(const Graph::IGraphUs &) override;
};

} // namespace GraphIsomorphism
