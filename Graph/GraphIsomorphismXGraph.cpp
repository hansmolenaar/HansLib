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

std::weak_ordering XGraph::compareGraph(const IGraphCompare &other) const
{
    const XGraph &lhs = *this;
    const XGraph &rhs = dynamic_cast<const XGraph &>(other);
    std::weak_ordering result = lhs.m_comparers->compareGraph(*rhs.m_comparers);
    if (result != std::weak_ordering::equivalent)
    {
        return result;
    }

    result = lhs.m_comparesComplement->compareGraph(*rhs.m_comparesComplement);
    if (result != std::weak_ordering::equivalent)
    {
        return result;
    }

    result = lhs.getComparersLineGraph().compareCharacteristics(rhs.getComparersLineGraph());
    return result;
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

const Comparers &XGraph::getComparersLineGraph() const
{
    if (!m_comparesLineGraph)
    {
        m_lineGraph = std::make_unique<UndirectedGraph>(UndirectedGraph::CreateLineGraph(getGraph()));
        m_comparesLineGraph = std::make_unique<Comparers>(*m_lineGraph);
    }
    return *m_comparesLineGraph;
}

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! Factory

std::unique_ptr<IGraphCompare> XGraphFactory::createGraphCompare(const Graph::IGraphUs &graph)
{
    return std::make_unique<XGraph>(graph);
}
