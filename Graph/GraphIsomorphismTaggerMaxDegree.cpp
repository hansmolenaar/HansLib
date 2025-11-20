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

TagEntry TaggerMaxDegree::getVertexTag(GraphVertex v) const
{
    if (!m_fullyConnected2id.contains(v))
    {
        return 0;
    }
    return m_fullyConnected2id.at(v);
}

const Tag &TaggerMaxDegree::getGraphTag() const
{
    return m_graphTag;
}

const Graph::IGraphUs &TaggerMaxDegree::getGraph() const
{
    return m_graph;
}

std::weak_ordering TaggerMaxDegree::compare(GraphVertex vertex0, GraphVertex vertex1) const
{
    return getVertexTag(vertex0) <=> getVertexTag(vertex1);
}

std::weak_ordering TaggerMaxDegree::compareOtherGraph(GraphVertex vertex0, const IVertexCompare &other,
                                                      GraphVertex vertex1) const
{
    return getVertexTag(vertex0) <=> dynamic_cast<const TaggerMaxDegree &>(other).getVertexTag(vertex1);
}

// !!!!!!!!!!!!!!!!!

std::unique_ptr<ITagger> TaggerMaxDegreeFactory::createTagger(const Graph::IGraphUs &graph)
{
    return std::make_unique<TaggerMaxDegree>(graph);
}
