#include "GraphIsomorphismTaggerDegree.h"
#include "Defines.h"
#include "GraphIsomorphismTagFlyweight.h"

using namespace GraphIsomorphism;

GraphIsomorphism::TaggerDegree::TaggerDegree(const Graph::IGraphUS &graph)
    : m_graph(graph), m_degreeSequenceTag(graph.getNumVertices())
{
    const auto nVertices = m_graph.getNumVertices();
    for (GraphVertex v = 0; v < nVertices; ++v)
    {
        m_degreeSequenceTag[v] = m_graph.getDegree(v);
    }
    str::sort(m_degreeSequenceTag);
}

bool TaggerDegree::isVertexTagger() const
{
    return true;
}

const Tag &TaggerDegree::getVertexTag(GraphVertex vertex) const
{
    return TagFlyweight::getSingleEntryTag(m_graph.getDegree(vertex));
}

bool TaggerDegree::isGraphTagger() const
{
    return true;
}

const Tag &TaggerDegree::getGraphTag() const
{
    return m_degreeSequenceTag;
}

// !!!!!!!!!!!!! FACTORY

std::unique_ptr<ITaggeR> TaggerDegreeFactory::create(const Graph::IGraphUS &graph)
{
    std::unique_ptr<ITaggeR> retval = std::make_unique<TaggerDegree>(graph);
    return retval;
}
