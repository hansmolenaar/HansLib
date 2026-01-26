#pragma once

#include "GraphIsomorphismComparers.h"
#include "GraphIsomorphismGrouping.h"
#include "UndirectedGraph.h"

namespace GraphIsomorphism
{

class XGraph
{
  public:
    explicit XGraph(const Graph::IGraphUs &);

    std::weak_ordering operator<=>(const XGraph &) const;
    bool operator==(const XGraph &) const;

    const VertexGrouping &getVertexGrouping() const;
    const Graph::IGraphUs &getGraph() const;
    const Graph::IGraphUs &getGraphComplement() const;

  private:
    Graph::UndirectedGraph m_complement;
    std::shared_ptr<Comparers> m_comparers;
    std::shared_ptr<Comparers> m_comparesComplement;
    VertexGrouping m_grouping;
};

} // namespace GraphIsomorphism
