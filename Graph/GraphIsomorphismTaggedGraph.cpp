#include "GraphIsomorphismTaggedGraph.h"
#include "Defines.h"
#include "GraphIsomorphismComparersFactory.h"
#include "GraphIsomorphismUtils.h"
#include "Iota.h"
#include "MyAssert.h"
#include "Single.h"

using namespace Graph;
using namespace GraphIsomorphism;
using namespace Utilities;

TaggedGraph::TaggedGraph(const Graph::IGraphUs &graph) : m_compare(graph)
{
}

std::weak_ordering TaggedGraph::compareOtherGraph(const IGraphCompare &other) const
{
    return m_compare.compareOtherGraph(dynamic_cast<const TaggedGraph &>(other).m_compare);
}

const Graph::IGraphUs &TaggedGraph::getGraph() const
{
    return m_compare.getGraph();
}

std::weak_ordering TaggedGraph::compareVertexOtherGraph(Vertex v0, const IVertexCompare &otherCompare, Vertex v1) const
{
    const TaggedGraph &otherTaggedGraph = dynamic_cast<const TaggedGraph &>(otherCompare);
    return m_compare.compareVertexOtherGraph(v0, otherTaggedGraph.m_compare, v1);
}

std::weak_ordering TaggedGraph::operator<=>(const TaggedGraph &rhs) const
{
    return m_compare <=> rhs.m_compare;
}

bool TaggedGraph::operator==(const TaggedGraph &rhs) const
{
    return (*this <=> rhs) == 0;
}

const Grouping<Vertex> &TaggedGraph::getVertexGrouping() const
{
    return m_compare.getVertexGrouping();
}

std::unique_ptr<ICompare> CompareTaggedGraphFactory::createCompare(const Graph::IGraphUs &graph)
{
    return std::make_unique<TaggedGraph>(graph);
}
