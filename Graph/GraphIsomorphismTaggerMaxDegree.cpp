#include "GraphIsomorphismTaggerMaxDegree.h"
#include "GraphIsomorphismTagFlyweight.h"

using namespace GraphIsomorphism;

TaggerMaxDegree::TaggerMaxDegree(const Graph::IGraphUs &graph) : m_graph(graph)
{
    const auto maxDegree = m_graph.getNumVertices() - 1;
    auto vertexIsFullyConnected = [&graph, maxDegree](GraphVertex vv) { return graph.getDegree(vv) == maxDegree; };
    for (GraphVertex v : graph.getVertexRange() | stv::filter(vertexIsFullyConnected))
    {
        m_fullyConnected2id[v] = m_fullyConnected2id.size() + 1;
    }
    m_graphTag = TagFlyweight::getSingleEntryTag(m_fullyConnected2id.size());
}

const Tag &TaggerMaxDegree::getVertexTag(GraphVertex v) const
{
    if (!m_fullyConnected2id.contains(v))
    {
        return TagFlyweight::getSingleEntryTag(0);
    }
    return TagFlyweight::getSingleEntryTag(m_fullyConnected2id.at(v));
}

GraphVertex TaggerMaxDegree ::getNumVertices() const
{
    return m_graph.getNumVertices();
}

const Tag &TaggerMaxDegree::getGraphTag() const
{
    return m_graphTag;
}

const Graph::IGraphUs &TaggerMaxDegree::getGraph() const
{
    return m_graph;
}

bool TaggerMaxDegree::less(GraphVertex vertex0, GraphVertex vertex1) const
{
    return m_graph.getDegree(vertex0) < m_graph.getDegree(vertex1);
}

bool TaggerMaxDegree::equal(GraphVertex vertex0, const IVertexCompare &other, GraphVertex vertex1) const
{
    return m_fullyConnected2id.contains(vertex0) ==
           dynamic_cast<const TaggerMaxDegree &>(other).m_fullyConnected2id.contains(vertex1);
}

// !!!!!!!!!!!!!!!!!

std::unique_ptr<ITagger> TaggerMaxDegreeFactory::createTagger(const Graph::IGraphUs &graph)
{
    return std::make_unique<TaggerMaxDegree>(graph);
}
