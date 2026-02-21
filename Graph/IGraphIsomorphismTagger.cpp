#include "IGraphIsomorphismTagger.h"
#include "Iota.h"

using namespace Graph;
using namespace GraphIsomorphism;

const IVertexCompare *ICompare::getVertexCompare() const
{
    return dynamic_cast<const IVertexCompare *>(this);
}

const IGraphCompare *ICompare::getGraphCompare() const
{
    return dynamic_cast<const IGraphCompare *>(this);
}

std::weak_ordering IVertexCompare::compareGraph(const IVertexCompare &rhs) const
{
    const IVertexCompare &lhs = *this;
    const auto &lhsGrouping = lhs.getVertexGrouping();
    const auto &rhsGrouping = rhs.getVertexGrouping();
    std::weak_ordering result = lhsGrouping.getGroupSizes() <=> rhsGrouping.getGroupSizes();
    if (result != std::weak_ordering::equivalent)
    {
        return result;
    }

    for (size_t n : Iota::GetRange(lhsGrouping().size()))
    {
        const Vertex v0 = lhsGrouping().at(n).front();
        const Vertex v1 = rhsGrouping().at(n).front();
        result = compareVertexOtherGraph(v0, rhs, v1);
        if (result != std::weak_ordering::equivalent)
        {
            return result;
        }
    }
    return result;
}
