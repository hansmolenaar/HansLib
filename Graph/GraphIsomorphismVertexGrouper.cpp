#include "GraphIsomorphismVertexGrouper.h"
#include "Defines.h"
#include "UndirectedGraph.h"

#include <map>

using namespace GraphIsomorphism;
using namespace Graph;

namespace
{
} // namespace

VertexGrouper::VertexGrouper(const IVertexTagger &tagger)
    : m_numVertices(tagger.getNumVertices()), m_grouping(IGraphUs::getVertexRange(m_numVertices), tagger)
{
}

const std::vector<Tag> &VertexGrouper::getTags() const
{
    return m_grouping.getTags();
}

const std::vector<GraphVertex> &VertexGrouper::getGroupMembers(const Tag &tag) const
{
    return m_grouping.getGroupMembers(tag);
}

int VertexGrouper::countUnique() const
{
    return m_grouping.countUnique();
}

bool VertexGrouper::isResolved() const
{
    return countUnique() == m_grouping.getTags().size();
}

GraphVertex VertexGrouper::getNumVertices() const
{
    return m_numVertices;
}

bool VertexGrouper::operator==(const GraphIsomorphism::VertexGrouper &grouper1) const
{
    return (*this <=> grouper1) == 0;
}

std::weak_ordering VertexGrouper::operator<=>(const VertexGrouper &grouper1) const
{
    const VertexGrouper &grouper0 = *this;

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
