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
    : TaggerDistance(std::make_shared<UndirectedGraphDistance>(graph),
                     std::make_shared<UndirectedGraphTriangles>(graph))
{
}

Tag TaggerDistance::getTag(Vertex v) const
{
    const auto &atDistance = (*m_distances)(v);
    // Skip first entry, always 1
    Tag retval(atDistance.size() - 1);
    std::transform(atDistance.begin() + 1, atDistance.end(), retval.begin(), [](const auto &ad) { return ad.size(); });
    return retval;
}

const Tag &TaggerDistance::getVertexTag(Vertex v) const
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

std::weak_ordering TaggerDistance::compareOtherGraph(Vertex vertex0, const IVertexCompare &other,
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

    for (size_t s = 0; s < size0; ++s)
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

std::unique_ptr<ITagger> TaggerDistanceFactory::createTagger(const Graph::IGraphUs &graph)
{
    return std::make_unique<TaggerDistance>(graph);
}
