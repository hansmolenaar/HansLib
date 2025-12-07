#include "IGraphUs.h"
#include "Defines.h"

#include <functional>

using namespace Graph;

std::vector<Vertex> IGraphUs::getDegreeSequence() const
{
    const auto numVertices = getNumVertices();
    std::vector<Vertex> result(numVertices);
    for (Vertex v = 0; v < numVertices; ++v)
    {
        result[v] = getDegree(v);
    }

    return result;
}

std::vector<Vertex> IGraphUs::getSortedDegreeSequence() const
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

Vertex IGraphUs::getNumberOfComponents() const
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
    const Vertex maxNumEdges = nVertices * (nVertices - 1) / 2;
    return getNumEdges() == maxNumEdges;
}

std::vector<Vertex> IGraphUs::getUniversalVertices() const
{
    std::vector<Vertex> result;
    const auto nVertices = getNumVertices();
    const auto maxDegree = nVertices - 1;
    str::copy_if(str::iota_view(Vertex{0}, nVertices), std::back_inserter(result),
                 [this, maxDegree](Vertex v) { return getDegree(v) == maxDegree; });
    return result;
}

std::vector<std::array<Vertex, 2>> IGraphUs::getAllSortedEdges() const
{
    const auto nVertices = getNumVertices();
    std::vector<std::array<Vertex, 2>> result;
    std::vector<Vertex> neighbors;
    for (Vertex v = 0; v < nVertices; ++v)
    {
        setAdjacentVertices(v, neighbors);
        for (Vertex ngb : neighbors)
        {
            if (ngb > v)
            {
                result.emplace_back(std::array<Vertex, 2>{v, ngb});
            }
        }
    }

    // Sorted/unique by construction
    return result;
}

bool IGraphUs::isClique(std::vector<Vertex> vertices) const
{
    str::sort(vertices);
    vertices.erase(std::unique(vertices.begin(), vertices.end()), vertices.end());

    if (vertices.size() < 2)
    {
        return false;
    }
    const size_t maxIntersectionSize = vertices.size() - 1;
    std::vector<Vertex> neighbors;
    std::vector<Vertex> intersection;

    for (Vertex v : vertices)
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
