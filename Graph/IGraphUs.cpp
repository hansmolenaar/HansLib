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

std::vector<GraphVertex> IGraphUs::getFullyConnectedVertices() const
{
    std::vector<GraphVertex> result;
    const auto nVertices = getNumVertices();
    const auto maxDegree = nVertices - 1;
    str::copy_if(str::iota_view(GraphVertex{0}, nVertices), std::back_inserter(result),
                 [this, maxDegree](GraphVertex v) { return getDegree(v) == maxDegree; });
    return result;
}
