#include "GraphIsomorphismXGraph.h"
#include "GraphIsomorphismGrouping.h"
#include "MyAssert.h"

using namespace Graph;
using namespace GraphIsomorphism;

XGraph::XGraph(const Graph::IGraphUs &graph)
    : m_complement(UndirectedGraph::CreateComplement(graph)), m_comparers(std::make_shared<Comparers>(graph)),
      m_comparesComplement(std::make_shared<Comparers>(m_complement)),
      m_grouping(VertexGrouping::Combine(m_comparers->getVertexGrouping(), m_comparesComplement->getVertexGrouping()))
{
}

std::weak_ordering XGraph::operator<=>(const XGraph &other) const
{
    std::weak_ordering result = *m_comparers <=> *other.m_comparers;
    if (result != std::weak_ordering::equivalent)
    {
        return result;
    }

    result = *m_comparesComplement <=> *other.m_comparesComplement;
    return result;
}

bool XGraph::operator==(const XGraph &other) const
{
    if (!(*m_comparers == *other.m_comparers))
    {
        return false;
    }

    return *m_comparesComplement == *other.m_comparesComplement;
}

const VertexGrouping &XGraph::getVertexGrouping() const
{
    return m_grouping;
}

const Graph::IGraphUs &XGraph::getGraph() const
{
    return m_comparers->getGraph();
}

const Graph::IGraphUs &XGraph::getGraphComplement() const
{
    return m_comparesComplement->getGraph();
}
