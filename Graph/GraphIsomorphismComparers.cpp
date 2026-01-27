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

std::weak_ordering Comparers::compareCharacteristics(const ICharacteristicsCompare &other) const
{
    return m_characteristicsComparers.compareCharacteristics(
        dynamic_cast<const Comparers &>(other).m_characteristicsComparers);
}

std::weak_ordering Comparers::compareVertexOtherGraph(Graph::Vertex v0, const IVertexCompare &other,
                                                      Graph::Vertex v1) const
{
    return m_vertexComparers.compareVertexOtherGraph(v0, dynamic_cast<const Comparers &>(other).m_vertexComparers, v1);
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

std::weak_ordering Comparers::operator<=>(const Comparers &other) const
{
    auto result = m_characteristicsComparers.compareCharacteristics(other.m_characteristicsComparers);
    if (result != std::weak_ordering::equivalent)
    {
        return result;
    }
    return m_vertexComparers <=> other.m_vertexComparers;
}

bool Comparers::operator==(const Comparers &other) const
{
    return *this <=> other == std::weak_ordering::equivalent;
}
