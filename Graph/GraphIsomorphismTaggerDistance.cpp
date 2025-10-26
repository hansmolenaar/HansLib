#include "GraphIsomorphismTaggerDistance.h"
#include "Defines.h"
#include "UndirectedGraphDistance.h"

using namespace GraphIsomorphism;

GraphIsomorphism::TaggerDistance::TaggerDistance(const UndirectedGraphDistance &distances) : m_distances(distances)
{
}

const UndirectedGraph &TaggerDistance::getGraph() const
{
    return m_distances.getGraph();
}

VertexTag TaggerDistance::getTag(GraphVertex v) const
{
    const auto &atDistance = m_distances(v);
    // Skip first entry, always 1
    VertexTag retval(atDistance.size() - 1);
    std::transform(atDistance.begin() + 1, atDistance.end(), retval.begin(), [](const auto &ad) { return ad.size(); });
    return retval;
}
