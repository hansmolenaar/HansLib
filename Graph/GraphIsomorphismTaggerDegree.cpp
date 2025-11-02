#include "GraphIsomorphismTaggerDegree.h"
#include "Defines.h"
#include "GraphIsomorphismTagFlyweight.h"

using namespace GraphIsomorphism;

GraphIsomorphism::TaggerDegree::TaggerDegree(const Graph::IGraphUS &graph)
    : m_graph(graph), m_degreeSequenceTag(graph.getNumVertices())
{
    // TODO getSortedDe...
    const auto nVertices = m_graph.getNumVertices();
    for (GraphVertex v = 0; v < nVertices; ++v)
    {
        m_degreeSequenceTag[v] = m_graph.getDegree(v);
    }
    str::sort(m_degreeSequenceTag);
}

const Tag &TaggerDegree::getGraphTag() const
{
    return m_degreeSequenceTag;
}

const Tag &TaggerDegree::getVertexTag(GraphVertex v) const
{
    return TagFlyweight::getSingleEntryTag(m_graph.getDegree(v));
}

GraphVertex TaggerDegree::getNumVertices() const
{
    return m_graph.getNumVertices();
}
// !!!!!!!!!!!!! FACTORY

std::unique_ptr<IGraphTagger> TaggerDegreeFactory::createGraphTagger(const Graph::IGraphUS &graph)
{
    std::unique_ptr<IGraphTagger> retval = std::make_unique<TaggerDegree>(graph);
    return retval;
}
