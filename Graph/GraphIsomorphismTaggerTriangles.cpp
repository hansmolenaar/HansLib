#include "GraphIsomorphismTaggerTriangles.h"
#include "Defines.h"
#include "GraphIsomorphismTagFlyweight.h"
#include "GraphIsomorphismUtils.h"

using namespace GraphIsomorphism;

namespace
{
std::vector<std::array<GraphVertex, 2>> getAllEdges(const std::vector<std::vector<GraphVertex>> &allNeighbors)
{
    std::vector<std::array<GraphVertex, 2>> result;
    const auto nVertices = allNeighbors.size();
    for (GraphVertex v = 0; v < nVertices; ++v)
    {
        for (GraphVertex ngb : allNeighbors.at(v))
        {
            if (ngb > v)
            {
                result.emplace_back(std::array<GraphVertex, 2>{v, ngb});
            }
        }
    }
    // Sorted/unique by construction
    return result;
}
}; // namespace

TaggerTriangles::TaggerTriangles(const Graph::IGraphUs &graph)
    : m_graph(graph), m_countPerVertex(graph.getNumVertices())
{
    const auto allTriangles = getAllTriangles(graph);
    for (const auto &triangle : allTriangles)
    {
        for (GraphVertex vertex : triangle)
        {
            m_countPerVertex.at(vertex) += 1;
        }
    }
    m_graphTag = CondenseSizeSequence(m_countPerVertex);
}

const Tag &TaggerTriangles::getGraphTag() const
{
    return m_graphTag;
}

const Tag &TaggerTriangles::getVertexTag(GraphVertex v) const
{
    return TagFlyweight::getSingleEntryTag(m_countPerVertex.at(v));
}

GraphVertex TaggerTriangles ::getNumVertices() const
{
    return m_countPerVertex.size();
}

std::vector<std::array<GraphVertex, 3>> TaggerTriangles::getAllTriangles(const Graph::IGraphUs &graph)
{
    const auto nVertices = graph.getNumVertices();
    std::vector<std::vector<GraphVertex>> allNeighbors(nVertices);
    for (GraphVertex v = 0; v < nVertices; ++v)
    {
        graph.setAdjacentVertices(v, allNeighbors.at(v));
    }

    std::vector<std::array<GraphVertex, 3>> result;
    std::vector<GraphVertex> commonVertices;
    for (const auto &edge : getAllEdges(allNeighbors))
    {
        commonVertices.clear();
        str::set_intersection(allNeighbors.at(edge[0]), allNeighbors.at(edge[1]), std::back_inserter(commonVertices));
        for (GraphVertex commonVertex : commonVertices)
        {
            if (commonVertex > edge[1])
            {
                result.emplace_back(std::array<GraphVertex, 3>{edge[0], edge[1], commonVertex});
            }
        }
    }

    return result;
}

const Graph::IGraphUs &TaggerTriangles::getGraph() const
{
    return m_graph;
}

std::weak_ordering TaggerTriangles::compare(GraphVertex vertex0, GraphVertex vertex1) const
{
    return getVertexTag(vertex0) <=> getVertexTag(vertex1);
}

std::weak_ordering TaggerTriangles::compareOtherGraph(GraphVertex vertex0, const IVertexCompare &other,
                                                      GraphVertex vertex1) const
{
    return getVertexTag(vertex0) <=> dynamic_cast<const TaggerTriangles &>(other).getVertexTag(vertex1);
}

std::unique_ptr<ITagger> TaggerTrianglesFactory::createTagger(const Graph::IGraphUs &graph)
{
    return std::make_unique<TaggerTriangles>(graph);
}
