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
