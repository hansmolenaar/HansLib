#include "GraphIsomorphismTaggerTwins.h"
#include "GraphIsomorphismTagFlyweight.h"
#include "GraphIsomorphismUtils.h"
#include "MyAssert.h"
#include "UndirectedGraphTwins.h"

using namespace Graph;
using namespace GraphIsomorphism;
using namespace Utilities;

namespace
{
void SetVertexTags(const std::vector<std::vector<Vertex>> &allTwins, size_t pos, std::map<Graph::Vertex, Tag> &tagMap)
{
    for (const auto &twins : allTwins)
    {
        for (Vertex v : twins)
        {
            if (!tagMap.contains(v))
            {
                tagMap[v] = Tag(2);
            }
            tagMap.at(v).at(pos) = twins.size();
        }
    }
}

Tag ToTag(size_t numVertices, const std::vector<std::vector<Vertex>> &allTwins)
{
    std::vector<size_t> sizes(numVertices, 0);
    for (const auto &twins : allTwins)
    {
        for (Vertex v : twins)
        {
            sizes.at(v) = twins.size();
        }
    }

    return CondenseSizeSequence(sizes);
}

} // namespace

TaggerTwins::TaggerTwins(const Graph::IGraphUs &graph) : m_graph(graph)
{
    const UndirectedGraphTwins twins(m_graph);
    m_graphTag = ToTag(m_graph.getNumVertices(), twins.getFalseTwins());
    for (TagEntry entry : ToTag(m_graph.getNumVertices(), twins.getTrueTwins()))
    {
        m_graphTag.push_back(entry);
    }

    SetVertexTags(twins.getFalseTwins(), 0, m_vertexTwinsTag);
    SetVertexTags(twins.getTrueTwins(), 1, m_vertexTwinsTag);
    m_vertexGrouping = VertexGrouping(m_graph.getVertexRange(), VertexLess{*this});
}

const Tag &TaggerTwins::getGraphTag() const
{
    return m_graphTag;
}

std::weak_ordering TaggerTwins::compareCharacteristics(const ICharacteristicsCompare &otherComparer) const
{
    const auto &other = dynamic_cast<const TaggerTwins &>(otherComparer);
    return getGraphTag() <=> other.getGraphTag();
}

const Graph::IGraphUs &TaggerTwins::getGraph() const
{
    return m_graph;
}

const Tag &TaggerTwins::getVertexTag(Graph::Vertex v) const
{
    auto found = m_vertexTwinsTag.find(v);

    if (found == m_vertexTwinsTag.end())
    {
        return TagFlyweight::getEmptyTag();
    }
    return found->second;
}

std::weak_ordering TaggerTwins::compareVertexOtherGraph(Graph::Vertex vertex1, const IVertexCompare &other,
                                                        Graph::Vertex vertex2) const
{
    const TaggerTwins &cmp2 = dynamic_cast<const TaggerTwins &>(other);
    return getVertexTag(vertex1) <=> cmp2.getVertexTag(vertex2);
}

const VertexGrouping &TaggerTwins::getVertexGrouping() const
{
    return m_vertexGrouping;
}

// !!!!!!!!!!!!! FACTORY

std::unique_ptr<ICompare> CompareTwinsFactory::createCompare(const Graph::IGraphUs &graph) const
{
    return std::make_unique<TaggerTwins>(graph);
}
