#include "GraphIsomorphismTaggerDegree.h"
#include "Defines.h"
#include "GraphIsomorphismTagFlyweight.h"
#include "GraphIsomorphismUtils.h"

using namespace GraphIsomorphism;

GraphIsomorphism::TaggerDegree::TaggerDegree(const Graph::IGraphUs &graph) : m_graph(graph)
{
    str::copy(GraphIsomorphismUtils::CondenseSizeSequence(graph.getDegreeSequence()),
              std::back_inserter(m_degreeSequenceTag));
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

std::unique_ptr<IGraphTagger> TaggerDegreeFactory::createGraphTagger(const Graph::IGraphUs &graph)
{
    std::unique_ptr<IGraphTagger> retval = std::make_unique<TaggerDegree>(graph);
    return retval;
}
