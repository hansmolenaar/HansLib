#include "GraphIsomorphismTaggerTriangles.h"
#include "Defines.h"

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
{
}

const Tag &TaggerTriangles::getGraphTag() const
{
    return m_graphTag;
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
