#include "GraphIsomorphismVertexComparers.h"

#include "IGraphUs.h"
#include "MyAssert.h"

using namespace Graph;
using namespace GraphIsomorphism;
using namespace Utilities;

namespace
{
struct CompareAll
{
    bool operator()(Vertex v0, Vertex v1) const
    {
        for (const auto *c : Comparers)
        {
            const auto cmp = c->compare(v0, v1);
            if (cmp == std::weak_ordering::less)
            {
                return true;
            }
            else if (cmp == std::weak_ordering::greater)
            {
                return false;
            }
        }
        return false; // equal
    }

    const std::vector<const IVertexCompare *> Comparers;
};

GraphIsomorphism::Grouping<Vertex> CreateGrouping(std::vector<const GraphIsomorphism::IVertexCompare *> comparers)
{
    const auto &graph = comparers.front()->getGraph();
    return GraphIsomorphism::Grouping<Vertex>(graph.getVertexRange(), CompareAll{comparers});
}

} // namespace

VertexComparers::VertexComparers(std::vector<const IVertexCompare *> comparers)
    : m_vertexComparers(comparers), m_grouping(CreateGrouping(comparers))
{
    size_t groupPos = 0;
    for (const auto &group : m_grouping())
    {
        for (Vertex vertex : group)
        {
            m_vertex2group[vertex] = groupPos;
        }
        ++groupPos;
    }
}

const IGraphUs &VertexComparers::getGraph() const
{
    return m_vertexComparers.front()->getGraph();
}

std::weak_ordering VertexComparers::compareVertexOtherGraph(Vertex v0, const IVertexCompare &otherCompare,
                                                            Vertex v1) const
{
    const VertexComparers *other = dynamic_cast<const VertexComparers *>(&otherCompare);
    if (this == other)
    {
        return m_vertex2group.at(v0) <=> m_vertex2group.at(v1);
    }

    const auto nComparers = m_vertexComparers.size();
    std::weak_ordering result = std::weak_ordering::equivalent;
    MyAssert(other->m_vertexComparers.size() == nComparers);
    for (size_t n : Iota::GetRange(nComparers))
    {
        const IVertexCompare *compare0 = m_vertexComparers.at(n);
        const IVertexCompare *compare1 = other->m_vertexComparers.at(n);
        result = compare0->compareVertexOtherGraph(v0, *compare1, v1);
        if (result != std::weak_ordering::equivalent)
        {
            return result;
        }
    }
    return result;
}

std::weak_ordering VertexComparers::operator<=>(const VertexComparers &other) const
{
    std::weak_ordering result = m_grouping.getGroupSizes() <=> other.m_grouping.getGroupSizes();
    if (result != std::weak_ordering::equivalent)
    {
        return result;
    }

    const auto &groups0 = m_grouping();
    const auto &groups1 = other.m_grouping();

    for (size_t n : Iota::GetRange(groups0.size()))
    {
        const Vertex v0 = groups0.at(n).front();
        const Vertex v1 = groups1.at(n).front();
        result = compareVertexOtherGraph(v0, other, v1);
        if (result != std::weak_ordering::equivalent)
        {
            return result;
        }
    }
    return result;
}

const Grouping<Graph::Vertex> &VertexComparers::getVertexGrouping() const
{
    return m_grouping;
}
