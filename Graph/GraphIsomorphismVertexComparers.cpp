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
}

const IGraphUs &VertexComparers::getGraph() const
{
    return m_vertexComparers.front()->getGraph();
}
