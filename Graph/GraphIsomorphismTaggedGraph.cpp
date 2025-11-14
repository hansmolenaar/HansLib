#include "GraphIsomorphismTaggedGraph.h"
#include "Defines.h"
#include "GraphIsomorphismUtils.h"
#include "MyAssert.h"

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

    m_vertexGroupTagsSorted = m_vertexGroupTags;
    str::sort(m_vertexGroupTagsSorted);
}

const Graph::IGraphUs &TaggedGraph::getGraph() const
{
    return m_graph;
}

// TODO TEST test TeSt
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

    return lhs.m_vertexGroupTagsSorted <=> rhs.m_vertexGroupTagsSorted;
}

bool TaggedGraph::operator==(const TaggedGraph &rhs) const
{
    const auto &lhs = *this;
    if (lhs.m_graphTags != rhs.m_graphTags)
    {
        return false;
    }

    MyAssert(lhs.m_vertexGroupers.size() == rhs.m_vertexGroupers.size());
    for (size_t n = 0; n < m_vertexGroupers.size(); ++n)
    {
        if (lhs.m_vertexGroupers.at(n) != rhs.m_vertexGroupers.at(n))
        {
            return true;
        }
    }

    return lhs.m_vertexGroupTagsSorted == rhs.m_vertexGroupTagsSorted;
}
