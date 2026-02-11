#include "GraphIsomorphismTaggerShortestCycle.h"

#include "GraphIsomorphismUtils.h"
#include "UndirectedGraphSmallestCycle.h"

using namespace Graph;
using namespace GraphIsomorphism;

TaggerShortestCycle::TaggerShortestCycle(const IGraphUs &graph)
    : m_graph(graph), m_shortestCycle(UndirectedGraphSmallestCycle(m_graph).getSmallestCycleLengths()),
      m_graphTag(CondenseSizeSequence(m_shortestCycle)),
      m_vertexGrouping(VertexGrouping(graph.getVertexRange(), VertexLess{*this}))
{
}

const IGraphUs &TaggerShortestCycle::getGraph() const
{
    return m_graph;
}

std::weak_ordering TaggerShortestCycle::compareVertexOtherGraph(Vertex lhsVertex, const IVertexCompare &other,
                                                                Vertex rhsVertex) const
{
    const TaggerShortestCycle &lhs = *this;
    const TaggerShortestCycle &rhs = dynamic_cast<const TaggerShortestCycle &>(other);
    return lhs.m_shortestCycle.at(lhsVertex) <=> rhs.m_shortestCycle.at(rhsVertex);
}

const Tag &TaggerShortestCycle::getGraphTag() const
{
    return m_graphTag;
}

const VertexGrouping &TaggerShortestCycle::getVertexGrouping() const
{
    return m_vertexGrouping;
}

// !!!!!!!!!!!!! FACTORY

std::unique_ptr<ICompare> CompareShortestCycleFactory::createCompare(const Graph::IGraphUs &graph) const
{
    return std::make_unique<TaggerShortestCycle>(graph);
}
