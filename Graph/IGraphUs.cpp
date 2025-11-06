#include "IGraphUs.h"
#include "Defines.h"

using namespace Graph;

std::vector<GraphVertex> IGraphUs ::getSortedDegreeSequence() const
{
    const auto numVertices = getNumVertices();
    std::vector<GraphVertex> result(numVertices);
    for (GraphVertex v = 0; v < numVertices; ++v)
    {
        result[v] = getDegree(v);
    }

    str::sort(result);
    return result;
}

bool IGraphUs ::isConnected() const
{
    const auto components = getConnectedComponents();
    return str::all_of(components, [](auto c) { return c == 0; });
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
