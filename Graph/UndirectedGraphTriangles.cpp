#include "UndirectedGraphTriangles.h"

using namespace Graph;

namespace
{

std::vector<std::array<GraphVertex, 2>> getAllSortedEdges(const std::vector<std::vector<GraphVertex>> &allNeighbors)
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

std::vector<std::array<GraphVertex, 3>> GetAllTriangles(const Graph::IGraphUs &graph)
{
    const auto nVertices = graph.getNumVertices();
    std::vector<std::vector<GraphVertex>> allNeighbors(nVertices);
    for (GraphVertex v = 0; v < nVertices; ++v)
    {
        graph.setAdjacentVertices(v, allNeighbors.at(v));
    }

    std::vector<std::array<GraphVertex, 3>> result;
    std::vector<GraphVertex> commonVertices;
    for (const auto &edge : getAllSortedEdges(allNeighbors))
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

} // namespace

UndirectedGraphTriangles::UndirectedGraphTriangles(const Graph::IGraphUs &graph) : m_graph(graph)
{
    const auto allTriangles = GetAllTriangles(graph);
    for (const auto &triangle : allTriangles)
    {
        for (GraphVertex vertex : triangle)
        {
            m_countPerVertex.at(vertex) += 1;
        }
    }
}
