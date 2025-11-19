#include "GraphIsomorphismTaggerDegree.h"
#include "Defines.h"
#include "GraphIsomorphismTagFlyweight.h"
#include "GraphIsomorphismUtils.h"

using namespace GraphIsomorphism;

GraphIsomorphism::TaggerDegree::TaggerDegree(const Graph::IGraphUs &graph) : m_graph(graph)
{
    str::copy(CondenseSizeSequence(graph.getDegreeSequence()), std::back_inserter(m_degreeSequenceTag));
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

const Graph::IGraphUs &TaggerDegree::getGraph() const
{
    return m_graph;
}

bool TaggerDegree::less(GraphVertex vertex0, GraphVertex vertex1) const
{
    return m_graph.getDegree(vertex0) < m_graph.getDegree(vertex1);
}

bool TaggerDegree::equal(GraphVertex vertex0, const IVertexCompare &other, GraphVertex vertex1) const
{
    return m_graph.getDegree(vertex0) == dynamic_cast<const TaggerDegree &>(other).m_graph.getDegree(vertex1);
}

// !!!!!!!!!!!!! FACTORY

std::unique_ptr<ITagger> TaggerDegreeFactory::createTagger(const Graph::IGraphUs &graph)
{
    return std::make_unique<TaggerDegree>(graph);
}
