#include "GraphIsomorphismGrouper.h"
#include "Defines.h"
#include "UndirectedGraph.h"

#include <map>

using namespace GraphIsomorphism;

namespace
{
struct Tag2Group
{
    int operator()(GraphVertex v)
    {
        const auto retval = tag2group.try_emplace(tagger.getTag(v), tag2group.size());
        return (retval.first)->second;
    }

    const IGraphIsomorphismTagger &tagger;
    std::map<VertexTag, int> tag2group;
};
} // namespace

Grouper::Grouper(const IGraphIsomorphismTagger &tagger) : m_graph(tagger.getGraph())
{
    const auto nVertices = m_graph.getNumVertices();

    std::map<VertexTag, std::vector<GraphVertex>> groups;
    for (GraphVertex v = 0; v < nVertices; ++v)
    {
        groups[tagger.getTag(v)].push_back(v);
    }

    for (const auto &itr : groups)
    {
        m_tags.emplace_back(itr.first);
        m_groups.emplace_back(itr.second);
    }
}

const std::vector<VertexTag> &Grouper::getTags() const
{
    return m_tags;
}

const std::vector<GraphVertex> *Grouper::getGroupMembers(const VertexTag &tag) const
{
    const auto found = str::find(m_tags, tag);
    if (found != m_tags.end())
    {
        const auto pos = std::distance(m_tags.begin(), found);
        return m_groups.data() + pos;
    }
    return {};
}
