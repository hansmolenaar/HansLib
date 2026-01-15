#pragma once

#include "GraphIsomorphismGrouping.h"
#include "GraphIsomorphismTaggedGraph.h"
#include "UndirectedGraph.h"

namespace GraphIsomorphism
{

class XGraph
{
  public:
    explicit XGraph(const Graph::IGraphUs &);

    std::weak_ordering operator<=>(const XGraph &) const;
    bool operator==(const XGraph &) const;

    const Grouping<Graph::Vertex> &getVertexGrouping() const;
    const Graph::IGraphUs &getGraph() const;
    const Graph::IGraphUs &getGraphComplement() const;

  private:
    Graph::UndirectedGraph m_complement;
    std::shared_ptr<TaggedGraph> m_taggedGraph;
    std::shared_ptr<TaggedGraph> m_taggedGraphComplement;
    Grouping<Graph::Vertex> m_grouping;
};

} // namespace GraphIsomorphism
