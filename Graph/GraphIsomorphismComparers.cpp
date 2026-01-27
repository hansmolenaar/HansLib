#include "GraphIsomorphismComparers.h"

#include "GraphIsomorphismComparersFactory.h"
#include "GraphIsomorphismUtils.h"

using namespace Graph;
using namespace GraphIsomorphism;

Comparers::Comparers(std::vector<std::unique_ptr<ICompare>> &&comparers)
    : m_comparers(std::move(comparers)), m_characteristicsComparers(selectCharacteristicsCompare(m_comparers)),
      m_vertexComparers(selectVertexCompare(m_comparers))
{
}

Comparers::Comparers(const Graph::IGraphUs &graph) : Comparers(ComparersFactory().getAllComparers(graph))
{
}

const VertexGrouping &Comparers::getVertexGrouping() const
{
    return m_vertexComparers.getVertexGrouping();
}

const Graph::IGraphUs &Comparers::getGraph() const
{
    return m_characteristicsComparers.getGraph();
}

std::weak_ordering Comparers::compareGraph(const IGraphCompare &other) const
{
    const auto &lhs = *this;
    const auto &rhs = dynamic_cast<const Comparers &>(other);
    auto result = lhs.m_characteristicsComparers.compareCharacteristics(rhs.m_characteristicsComparers);
    if (result != std::weak_ordering::equivalent)
    {
        return result;
    }
    return lhs.m_vertexComparers <=> rhs.m_vertexComparers;
}
