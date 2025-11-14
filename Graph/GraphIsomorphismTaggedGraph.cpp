#include "GraphIsomorphismTaggedGraph.h"
#include "GraphIsomorphismUtils.h"

using namespace Graph;
using namespace GraphIsomorphism;

TaggedGraph::TaggedGraph(const Graph::IGraphUs &graph) : m_graph(graph), m_taggers(getAllTaggers(m_graph))
{
    for (const auto *graphTagger : selectGraphTaggers(m_taggers))
    {
        m_graphTags.emplace_back(graphTagger->getGraphTag());
    }
    for (const auto *vertexTagger : selectVertexTaggers(m_taggers))
    {
        m_vertexGroupers.emplace_back(*vertexTagger);
    }
}
