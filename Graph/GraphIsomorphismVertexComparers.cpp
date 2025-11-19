#include "GraphIsomorphismVertexComparers.h"

#include "IGraphUs.h"

using namespace Graph;
using namespace GraphIsomorphism;

namespace
{
struct CompareAll
{
    bool operator()(GraphVertex v0, GraphVertex v1) const
    {
        for (const auto *c : Comparers)
        {
            if (c->less(v0, v1))
            {
                return true;
            }
        }
        return false;
    }

    const std::vector<const IVertexCompare *> Comparers;
};
} // namespace

VertexComparers::VertexComparers(std::vector<const IVertexCompare *> comparers)
    : m_vertexComparers(std::move(comparers)), m_grouping(getGraph().getVertexRange(), CompareAll{m_vertexComparers})
{
    size_t groupPos = 0;
    for (const auto &group : m_grouping())
    {
        for (GraphVertex vertex : group)
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

bool VertexComparers::less(GraphVertex v0, GraphVertex v1) const
{
    return m_vertex2group.at(v0) < m_vertex2group.at(v1);
}

bool VertexComparers::equal(GraphVertex v0, const IVertexCompare &otherCompare, GraphVertex v1) const
{
    const VertexComparers &other = dynamic_cast<const VertexComparers &>(otherCompare);
    const auto nComparers = m_vertexComparers.size();
    for (size_t n = 0; n < nComparers; ++n)
    {
        const IVertexCompare *compare0 = m_vertexComparers.at(n);
        const IVertexCompare *compare1 = other.m_vertexComparers.at(n);
        if (!compare0->equal(v0, *compare1, v1))
        {
            return false;
        }
    }
    return true;
}
