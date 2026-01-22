#include "GraphIsomorphismTaggerMaxDegree.h"
#include "GraphIsomorphismTagFlyweight.h"

using namespace Graph;
using namespace GraphIsomorphism;

TaggerMaxDegree::TaggerMaxDegree(const Graph::IGraphUs &graph) : m_graph(graph)
{
    const auto maxDegree = m_graph.getNumVertices() - 1;
    auto vertexIsUniversal = [&graph, maxDegree](Vertex vv) { return graph.getDegree(vv) == maxDegree; };
    for (Vertex v : graph.getVertexRange() | stv::filter(vertexIsUniversal))
    {
        m_universal2id[v] = m_universal2id.size() + 1;
    }
    m_graphTag = TagFlyweight::getSingleEntryTag(m_universal2id.size());
}

TagEntry TaggerMaxDegree::getVertexTag(Vertex v) const
{
    if (!m_universal2id.contains(v))
    {
        return 0;
    }
    return m_universal2id.at(v);
}

const Tag &TaggerMaxDegree::getGraphTag() const
{
    return m_graphTag;
}

const Graph::IGraphUs &TaggerMaxDegree::getGraph() const
{
    return m_graph;
}

std::weak_ordering TaggerMaxDegree::compareOtherGraph(Vertex vertex0, const IVertexCompare &other, Vertex vertex1) const
{
    return getVertexTag(vertex0) <=> dynamic_cast<const TaggerMaxDegree &>(other).getVertexTag(vertex1);
}

// !!!!!!!!!!!!!!!!!

std::unique_ptr<ITagger> TaggerMaxDegreeFactory::createTagger(const Graph::IGraphUs &graph)
{
    return std::make_unique<TaggerMaxDegree>(graph);
}

std::unique_ptr<ICompare> CompareMaxDegreeFactory::createCompare(const Graph::IGraphUs &graph)
{
    return std::make_unique<TaggerMaxDegree>(graph);
}
