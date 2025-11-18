#include "GraphIsomorphismTaggedGraph.h"
#include "Defines.h"
#include "GraphIsomorphismUtils.h"
#include "MyAssert.h"
#include "Single.h"

using namespace Graph;
using namespace GraphIsomorphism;
using namespace Utilities;

TaggedGraph::TaggedGraph(const Graph::IGraphUs &graph)
    : m_graph(graph), m_taggers(getAllTaggers(m_graph)), m_vertexGroupTags(graph.getNumVertices())
{
    for (const auto *graphTagger : selectGraphTaggers(m_taggers))
    {
        m_graphTags.emplace_back(graphTagger->getGraphTag());
    }
    for (const auto *vertexTagger : selectVertexTaggers(m_taggers))
    {
        m_vertexGroupers.emplace_back(*vertexTagger);
    }
    for (const auto &grouper : m_vertexGroupers)
    {
        TagEntry groupEntry = 0;
        for (const auto &tag : grouper.getTags())
        {
            ++groupEntry;
            for (GraphVertex v : grouper.getGroupMembers(tag))
            {
                m_vertexGroupTags.at(v).push_back(groupEntry);
            }
        }
    }

    auto getTag = [this](GraphVertex v) { return m_vertexGroupTags.at(v); };
    const TaggedGrouping<Tag, GraphVertex> grouping(m_graph.getVertexRange(), getTag);
    m_uniqueVertexAndGroupTag = grouping.getUniqueTags();

    str::sort(m_vertexGroupTags);
}

const Graph::IGraphUs &TaggedGraph::getGraph() const
{
    return m_graph;
}

std::weak_ordering TaggedGraph::operator<=>(const TaggedGraph &rhs) const
{
    const auto &lhs = *this;
    std::weak_ordering result = lhs.m_graphTags <=> rhs.m_graphTags;
    if (result != 0)
    {
        return result;
    }

    for (size_t n = 0; n < m_vertexGroupers.size(); ++n)
    {
        result = lhs.m_vertexGroupers.at(n) <=> rhs.m_vertexGroupers.at(n);

        if (result != 0)
        {
            return result;
        }
    }

    return lhs.m_vertexGroupTags <=> rhs.m_vertexGroupTags;
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

    for (const auto &itr : tg0.m_uniqueVertexAndGroupTag)
    {
        const auto &tag = itr.first;
        const GraphVertex vertex0 = tg0.m_uniqueVertexAndGroupTag.at(tag);
        const GraphVertex vertex1 = tg1.m_uniqueVertexAndGroupTag.at(tag);
        result.addPair(VertexPair{vertex0, vertex1});
    }

    return result;
}
