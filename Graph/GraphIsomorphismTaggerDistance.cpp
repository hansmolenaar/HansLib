#include "GraphIsomorphismTaggerDistance.h"
#include "Defines.h"
#include "UndirectedGraph.h"
#include "UndirectedGraphDistance.h"

using namespace GraphIsomorphism;

GraphIsomorphism::TaggerDistance::TaggerDistance(std::shared_ptr<UndirectedGraphDistance> distances)
    : m_distances(std::move(distances))
{
    const auto nVertices = m_distances->getNumVertices();
    for (auto v = 0; v < nVertices; ++v)
    {
        m_tags.emplace_back(getVertexTag(v));
    }
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

GraphVertex TaggerDistance::getNumVertices() const
{
    return m_tags.size();
}

// !!!!!!!!!!!!! FACTORY

std::unique_ptr<IVertexTagger> TaggerDistanceFactory::createVertexTagger(const Graph::IGraphUSC &graph)
{
    auto distances = std::make_shared<UndirectedGraphDistance>(graph);
    // return std::make_unique<TaggerDistance>(distances);
    return {};
}
