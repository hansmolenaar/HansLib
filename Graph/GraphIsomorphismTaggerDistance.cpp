#include "GraphIsomorphismTaggerDistance.h"
#include "Defines.h"
#include "GraphIsomorphismUtils.h"
#include "IGraphUsc.h"
#include "UndirectedGraph.h"
#include "UndirectedGraphDistance.h"
#include "UndirectedGraphTriangles.h"

using namespace Graph;
using namespace GraphIsomorphism;

GraphIsomorphism::TaggerDistance::TaggerDistance(std::shared_ptr<UndirectedGraphDistance> distances,
                                                 std::shared_ptr<Graph::UndirectedGraphTriangles> triangles)
    : m_distances(std::move(distances)), m_triangles(std::move(triangles))
{
    const auto nVertices = m_distances->getNumVertices();

    // Use maximum distances in the graph tag
    std::vector<size_t> maxDistances;
    for (auto v : Iota::GetRange(nVertices))
    {
        // Subtract 1, gives expected distance
        maxDistances.push_back((*m_distances)(v).size() - 1);
    }
    m_graphTag = CondenseSizeSequence(maxDistances);
    m_vertexGrouping = VertexGrouping(Iota::GetRange(nVertices), VertexLess{*this});
}

TaggerDistance::TaggerDistance(const Graph::IGraphUs &graph)
    : TaggerDistance(std::make_shared<UndirectedGraphDistance>(graph),
                     std::make_shared<UndirectedGraphTriangles>(graph))
{
}

const Tag &TaggerDistance::getGraphTag() const
{
    return m_graphTag;
}

std::weak_ordering TaggerDistance::compareCharacteristics(const ICharacteristicsCompare &otherComparer) const
{
    const auto &other = dynamic_cast<const TaggerDistance &>(otherComparer);
    return getGraphTag() <=> other.getGraphTag();
}

const IGraphUs &TaggerDistance::getGraph() const
{
    return m_distances->getGraph();
}

const VertexGrouping &TaggerDistance::getVertexGrouping() const
{
    return m_vertexGrouping;
}

namespace
{
std::vector<ScalarVertexValues> CollectVertexValues(const std::vector<Vertex> &vertices,
                                                    const UndirectedGraphTriangles &triangles)
{
    std::vector<ScalarVertexValues> result(vertices.size());
    str::transform(vertices, result.begin(), [&triangles](Vertex v) {
        return ScalarVertexValues{static_cast<TagEntry>(triangles.getGraph().getDegree(v)),
                                  static_cast<TagEntry>(triangles.numTrianglesAt(v)),
                                  static_cast<TagEntry>(triangles.numNeighborsInTrianglesAt(v))};
    });
    str::sort(result);
    return result;
}
} // namespace

std::weak_ordering TaggerDistance::compareVertexOtherGraph(Vertex vertex0, const IVertexCompare &other,
                                                           Vertex vertex1) const
{
    const auto &tagger0 = *this;
    const auto &tagger1 = dynamic_cast<const TaggerDistance &>(other);

    const auto &atDistance0 = (*tagger0.m_distances)(vertex0);
    const auto &atDistance1 = (*tagger1.m_distances)(vertex1);

    const auto size0 = atDistance0.size();
    std::weak_ordering retval = size0 <=> atDistance1.size();
    if (retval != std::weak_ordering::equivalent)
    {
        return retval;
    }

    for (size_t s : Iota::GetRange(size0))
    {
        const auto &vertices0 = atDistance0.at(s);
        const auto &vertices1 = atDistance1.at(s);
        const auto values0 = CollectVertexValues(vertices0, *tagger0.m_triangles);
        const auto values1 = CollectVertexValues(vertices1, *tagger1.m_triangles);
        retval = values0 <=> values1;
        if (retval != std::weak_ordering::equivalent)
        {
            return retval;
        }
    }
    return retval;
}

// !!!!!!!!!!!!! FACTORY

std::unique_ptr<ICompare> CompareDistanceFactory::createCompare(const Graph::IGraphUs &graph) const
{
    return std::make_unique<TaggerDistance>(graph);
}
