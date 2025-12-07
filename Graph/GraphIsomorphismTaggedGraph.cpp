#include "GraphIsomorphismTaggedGraph.h"
#include "Defines.h"
#include "GraphIsomorphismUtils.h"
#include "MyAssert.h"
#include "Single.h"

using namespace Graph;
using namespace GraphIsomorphism;
using namespace Utilities;

TaggedGraph::TaggedGraph(const Graph::IGraphUs &graph)
    : m_graph(graph), m_taggers(getAllTaggers(m_graph)), m_vertexComparers(selectVertexCompare(m_taggers)),
      m_grouping(graph.getVertexRange(), VertexLess{m_vertexComparers})
{
    for (const auto *graphTagger : selectGraphTaggers(m_taggers))
    {
        m_graphTags.emplace_back(graphTagger->getGraphTag());
    }
}

const Graph::IGraphUs &TaggedGraph::getGraph() const
{
    return m_graph;
}

std::weak_ordering TaggedGraph::operator<=>(const TaggedGraph &rhs) const
{
    const auto &lhs = *this;
    std::weak_ordering result = lhs.m_graphTags <=> rhs.m_graphTags;
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

Status TaggedGraph::tryConnect(const TaggedGraph &tg0, const TaggedGraph &tg1)
{
    const auto numVertices = tg0.getGraph().getNumVertices();
    Status result(numVertices);

    if (tg0 != tg1)
    {
        result.setFlag(Flag::NotIsomorphic);
        return result;
    }

    const auto uniqueValues0 = tg0.m_grouping.getUniqueValues();
    const auto uniqueValues1 = tg1.m_grouping.getUniqueValues();
    MyAssert(uniqueValues0.size() == uniqueValues1.size());
    for (const auto vertex01 : stv::zip(uniqueValues0, uniqueValues1))
    {
        result.addPair({std::get<0>(vertex01), std::get<1>(vertex01)});
    }

    return result;
}

const Grouping<Vertex> &TaggedGraph::getVertexGrouping() const
{
    return m_grouping;
}
