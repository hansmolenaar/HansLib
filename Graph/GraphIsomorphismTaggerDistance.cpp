#include "GraphIsomorphismTaggerDistance.h"
#include "Defines.h"
#include "GraphIsomorphismUtils.h"
#include "IGraphUsc.h"
#include "UndirectedGraph.h"
#include "UndirectedGraphDistance.h"

using namespace Graph;
using namespace GraphIsomorphism;

GraphIsomorphism::TaggerDistance::TaggerDistance(std::shared_ptr<UndirectedGraphDistance> distances)
    : m_distances(std::move(distances))
{
    const auto nVertices = m_distances->getNumVertices();
    for (auto v = 0; v < nVertices; ++v)
    {
        m_tags.emplace_back(getTag(v));
    }

    // Use maximum distances in the graph tag
    std::vector<size_t> maxDistances;
    for (auto v = 0; v < nVertices; ++v)
    {
        // Subtract 1, gives expected distance
        maxDistances.push_back((*m_distances)(v).size() - 1);
    }
    m_graphTag = CondenseSizeSequence(maxDistances);
}

TaggerDistance::TaggerDistance(const Graph::IGraphUs &graph)
    : TaggerDistance(std::make_shared<UndirectedGraphDistance>(graph))
{
}

Tag TaggerDistance::getTag(GraphVertex v) const
{
    const auto &atDistance = (*m_distances)(v);
    // Skip first entry, always 1
    Tag retval(atDistance.size() - 1);
    std::transform(atDistance.begin() + 1, atDistance.end(), retval.begin(), [](const auto &ad) { return ad.size(); });
    return retval;
}

const Tag &TaggerDistance::getVertexTag(GraphVertex v) const
{
    return m_tags.at(v);
}

const Tag &TaggerDistance::getGraphTag() const
{
    return m_graphTag;
}

const IGraphUs &TaggerDistance::getGraph() const
{
    return m_distances->getGraph();
}

std::weak_ordering TaggerDistance::compareOtherGraph(GraphVertex vertex0, const IVertexCompare &other,
                                                     GraphVertex vertex1) const
{
    return getVertexTag(vertex0) <=> dynamic_cast<const TaggerDistance &>(other).getVertexTag(vertex1);
}

// !!!!!!!!!!!!! FACTORY

std::unique_ptr<ITagger> TaggerDistanceFactory::createTagger(const Graph::IGraphUs &graph)
{
    return std::make_unique<TaggerDistance>(graph);
}
