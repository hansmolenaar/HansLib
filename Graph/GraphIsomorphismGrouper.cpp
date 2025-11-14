#include "GraphIsomorphismGrouper.h"
#include "Defines.h"
#include "Single.h"
#include "UndirectedGraph.h"

#include <map>

using namespace GraphIsomorphism;
using namespace Utilities;

namespace
{
const std::vector<GraphVertex> s_emptyMemberList;
} // namespace

Grouper::Grouper(const IVertexTagger &tagger) : m_numVertices(tagger.getNumVertices())
{
    std::map<Tag, std::vector<GraphVertex>> groups;
    for (GraphVertex v = 0; v < m_numVertices; ++v)
    {
        groups[tagger.getVertexTag(v)].push_back(v);
    }

    for (const auto &itr : groups)
    {
        m_tags.emplace_back(itr.first);
        m_groups.emplace_back(itr.second);
    }
}

const std::vector<Tag> &Grouper::getTags() const
{
    return m_tags;
}

const std::vector<GraphVertex> &Grouper::getGroupMembers(const Tag &tag) const
{
    const auto found = str::find(m_tags, tag);
    if (found != m_tags.end())
    {
        const auto pos = std::distance(m_tags.begin(), found);
        return m_groups.at(pos);
    }
    return s_emptyMemberList;
}

int Grouper::countUnique() const
{
    return str::count_if(m_groups, [](const auto &g) { return g.size() == 1; });
}

bool Grouper::isResolved() const
{
    return countUnique() == m_tags.size();
}

GraphVertex Grouper::getNumVertices() const
{
    return m_numVertices;
}

bool Grouper::operator==(const GraphIsomorphism::Grouper &grouper1 ) const
{
    const Grouper& grouper0 = *this;

    const auto &tags0 = grouper0.getTags();
    const auto &tags1 = grouper1.getTags();
    if (!str::equal(tags0, tags1))
    {
        return false;
    }
    for (const auto &tag : tags0)
    {
        const auto &members0 = grouper0.getGroupMembers(tag);
        const auto &members1 = grouper1.getGroupMembers(tag);
        if (members0.size() != members1.size())
        {
            return false;
        }
    }

    return true;
}

std::weak_ordering Grouper::operator<=>(const Grouper &grouper1) const
{
    const Grouper& grouper0 = *this;

    const auto &tags0 = grouper0.getTags();
    const auto &tags1 = grouper1.getTags();

    std::weak_ordering result = tags0.size() <=> tags1.size();
    if (result != 0)
    {
        return result;
    }

    for (size_t n = 0; n < tags0.size(); ++n)
    {
        result = tags0.at(n) <=> tags1.at(n);
        if (result != 0)
        {
            return result;
        }
    }

    for (const auto &tag : tags0)
    {
        const auto &members0 = grouper0.getGroupMembers(tag);
        const auto &members1 = grouper1.getGroupMembers(tag);
        result = members0.size() <=> members1.size();
        if (result != 0)
        {
            return result;
        }
    }

    return result;
}

void Grouper::updateVertexGroupTags(std::vector<Tag> &groupTags) const
{
    TagEntry groupEntry = 0;
    for (const auto &tag : getTags())
    {
        ++groupEntry;
        for (GraphVertex v : getGroupMembers(tag))
        {
            groupTags.at(v).push_back(groupEntry);
        }
    }
}

Status Grouper::compare(const Grouper &grouper0, const Grouper &grouper1)
{
    MyAssert(grouper0.getNumVertices() == grouper1.getNumVertices());
    Status result(grouper0.getNumVertices());

    if (grouper0 !=  grouper1)
    {
        result.setFlag(Flag::NotIsomorphic);
        return result;
    }

    const auto &tags = grouper0.getTags();
    for (const auto &tag : tags)
    {
        const auto &members0 = grouper0.getGroupMembers(tag);
        if (members0.size() == 1)
        {
            const auto &members1 = grouper1.getGroupMembers(tag);
            result.addPair(VertexPair{Single(members0), Single(members1)});
        }
    }

    return result;
}
