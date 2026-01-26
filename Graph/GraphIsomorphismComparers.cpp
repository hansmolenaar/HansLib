#include "GraphIsomorphismComparers.h"

#include "GraphIsomorphismComparersFactory.h"
#include "GraphIsomorphismUtils.h"

using namespace Graph;
using namespace GraphIsomorphism;

Comparers::Comparers(std::vector<std::unique_ptr<ICompare>> &&comparers)
    : m_comparers(std::move(comparers)), m_graphComparers(selectGraphCompare(m_comparers)),
      m_vertexComparers(selectVertexCompare(m_comparers))
{
}

Comparers::Comparers(const Graph::IGraphUs &graph) : Comparers(ComparersFactory().getAllComparers(graph))
{
}

const Grouping<Vertex> &Comparers::getVertexGrouping() const
{
    return m_vertexComparers.getVertexGrouping();
}

const Graph::IGraphUs &Comparers::getGraph() const
{
    return m_graphComparers.getGraph();
}

std::weak_ordering Comparers::compareCharacteristics(const ICharacteristicsCompare &other) const
{
    return m_graphComparers.compareCharacteristics(dynamic_cast<const Comparers &>(other).m_graphComparers);
}

std::weak_ordering Comparers::compareVertexOtherGraph(Graph::Vertex v0, const IVertexCompare &other,
                                                      Graph::Vertex v1) const
{
    return m_vertexComparers.compareVertexOtherGraph(v0, dynamic_cast<const Comparers &>(other).m_vertexComparers, v1);
}

std::weak_ordering Comparers::operator<=>(const Comparers &other) const
{
    auto result = m_graphComparers.compareCharacteristics(other.m_graphComparers);
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
