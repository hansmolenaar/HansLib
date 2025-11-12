#include "GraphIsomorphismTaggerMaxDegree.h"
#include "GraphIsomorphismTagFlyweight.h"

using namespace GraphIsomorphism;

TaggerMaxDegree::TaggerMaxDegree(const Graph::IGraphUs &graph) : m_numVertices(graph.getNumVertices())
{
    const auto maxDegree = m_numVertices - 1;
    for (GraphVertex v = 0; v < m_numVertices; ++v)
    {
        if (graph.getDegree(v) == maxDegree)
        {
            m_fullyConnected2id[v] = m_fullyConnected2id.size() + 1;
        }
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
    return m_numVertices;
}

const Tag &TaggerMaxDegree::getGraphTag() const
{
    return m_graphTag;
}

// !!!!!!!!!!!!!!!!!

std::unique_ptr<ITagger> TaggerMaxDegreeFactory::createTagger(const Graph::IGraphUs &graph)
{
    return std::make_unique<TaggerMaxDegree>(graph);
}
