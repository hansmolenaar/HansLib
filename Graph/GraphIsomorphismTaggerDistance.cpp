#include "GraphIsomorphismTaggerDistance.h"
#include "Defines.h"
#include "UndirectedGraphDistance.h"

using namespace GraphIsomorphism;

GraphIsomorphism::TaggerDistance::TaggerDistance(std::shared_ptr< UndirectedGraphDistance> distances) : m_distances(std::move(distances))
{
}

const UndirectedGraph &TaggerDistance::getGraph() const
{
    return m_distances->getGraph();
}

VertexTag TaggerDistance::getTag(GraphVertex v) const
{
    const auto &atDistance = (*m_distances)(v);
    // Skip first entry, always 1
    VertexTag retval(atDistance.size() - 1);
    std::transform(atDistance.begin() + 1, atDistance.end(), retval.begin(), [](const auto &ad) { return ad.size(); });
    return retval;
}

// !!!!!!!!!!!!! FACTORY

std::unique_ptr<ITagger> TaggerDistanceFactory::create(const UndirectedGraph &graph)
{
auto distances = std::make_shared<UndirectedGraphDistance>(graph);
    return std::make_unique<TaggerDistance>(distances);
}
