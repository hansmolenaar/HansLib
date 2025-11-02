#include "IGraphUS.h"
#include "Defines.h"

using namespace Graph;

std::vector<GraphVertex> IGraphUS ::getSortedDegreeSequence() const
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

bool IGraphUS ::isConnected() const
{
    const auto components = getConnectedComponents();
    return str::all_of(components, [](auto c) { return c == 0; });
}
