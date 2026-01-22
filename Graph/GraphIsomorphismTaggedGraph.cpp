#include "GraphIsomorphismTaggedGraph.h"
#include "Defines.h"
#include "GraphIsomorphismAllCompareFactories.h"
#include "GraphIsomorphismUtils.h"
#include "Iota.h"
#include "MyAssert.h"
#include "Single.h"

using namespace Graph;
using namespace GraphIsomorphism;
using namespace Utilities;

TaggedGraph::TaggedGraph(const Graph::IGraphUs &graph)
    : m_graph(graph), m_comparers(AllCompareFactories().getAllComparers(m_graph)),
      m_vertexComparers(selectVertexCompare(m_comparers)),
      m_grouping(graph.getVertexRange(), VertexLess{m_vertexComparers})
{
}

std::weak_ordering TaggedGraph::compareOtherGraph(const IGraphCompare &other) const
{
    std::weak_ordering result = std::weak_ordering::equivalent;
    const auto &tg = dynamic_cast<const TaggedGraph &>(other);
    const auto taggers0 = selectGraphTaggers(m_comparers);
    const auto taggers1 = selectGraphTaggers(tg.m_comparers);
    Utilities::MyAssert(taggers0.size() == taggers1.size());
    for (size_t n : Iota::GetRange(taggers0.size()))
    {
        result = taggers0.at(n)->getGraphTag() <=> taggers1.at(n)->getGraphTag();
        if (result != std::weak_ordering::equivalent)
        {
            return result;
        }
    }

    return result;
}

const Graph::IGraphUs &TaggedGraph::getGraph() const
{
    return m_graph;
}

std::weak_ordering TaggedGraph::operator<=>(const TaggedGraph &rhs) const
{
    const auto &lhs = *this;
    std::weak_ordering result = compareOtherGraph(dynamic_cast<const IGraphCompare &>(rhs));
    if (result != std::weak_ordering::equivalent)
    {
        return result;
    }

    return lhs.m_vertexComparers <=> rhs.m_vertexComparers;
}

bool TaggedGraph::operator==(const TaggedGraph &rhs) const
{
    return (*this <=> rhs) == 0;
}

const Grouping<Vertex> &TaggedGraph::getVertexGrouping() const
{
    return m_grouping;
}

std::unique_ptr<ICompare> CompareTaggedGraphFactory::createCompare(const Graph::IGraphUs &graph)
{
    return std::make_unique<TaggedGraph>(graph);
}
