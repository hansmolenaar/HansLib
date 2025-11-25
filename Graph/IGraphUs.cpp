#include "IGraphUs.h"
#include "Defines.h"

#include <functional>

using namespace Graph;

std::vector<GraphVertex> IGraphUs::getDegreeSequence() const
{
    const auto numVertices = getNumVertices();
    std::vector<GraphVertex> result(numVertices);
    for (GraphVertex v = 0; v < numVertices; ++v)
    {
        result[v] = getDegree(v);
    }

    return result;
}

std::vector<GraphVertex> IGraphUs::getSortedDegreeSequence() const
{
    auto result = getDegreeSequence();
    str::sort(result);
    return result;
}

bool IGraphUs::isConnected() const
{
    const auto components = getConnectedComponents();
    return str::all_of(components, [](auto c) { return c == 0; });
}

GraphVertex IGraphUs::getNumberOfComponents() const
{
    const auto components = getConnectedComponents();
    if (components.empty())
    {
        return 0;
    }
    return *str::max_element(components) + 1;
}

bool IGraphUs::isComplete() const
{
    const auto nVertices = getNumVertices();
    // Edge cases for clarity
    if (nVertices <= 1)
    {
        return true;
    }
    const GraphVertex maxNumEdges = nVertices * (nVertices - 1) / 2;
    return getNumEdges() == maxNumEdges;
}

std::vector<GraphVertex> IGraphUs::getFullyConnectedVertices() const
{
    std::vector<GraphVertex> result;
    const auto nVertices = getNumVertices();
    const auto maxDegree = nVertices - 1;
    str::copy_if(str::iota_view(GraphVertex{0}, nVertices), std::back_inserter(result),
                 [this, maxDegree](GraphVertex v) { return getDegree(v) == maxDegree; });
    return result;
}

std::vector<std::array<GraphVertex, 2>> IGraphUs::getAllSortedEdges() const
{
    const auto nVertices = getNumVertices();
    std::vector<std::array<GraphVertex, 2>> result;
    std::vector<GraphVertex> neighbors;
    for (GraphVertex v = 0; v < nVertices; ++v)
    {
        setAdjacentVertices(v, neighbors);
        for (GraphVertex ngb : neighbors)
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

bool IGraphUs::isClique(std::vector<GraphVertex> vertices) const
{
    str::sort(vertices);
    vertices.erase(std::unique(vertices.begin(), vertices.end()), vertices.end());

    if (vertices.size() < 2)
    {
        return false;
    }
    const size_t maxIntersectionSize = vertices.size() - 1;
    std::vector<GraphVertex> neighbors;
    std::vector<GraphVertex> intersection;

    for (GraphVertex v : vertices)
    {
        setAdjacentVertices(v, neighbors);
        intersection.clear();
        std::set_intersection(vertices.begin(), vertices.end(), neighbors.begin(), neighbors.end(),
                              std::back_inserter(intersection));
        if (intersection.size() < maxIntersectionSize)
        {
            return false;
        }
    }

    return true;
}
