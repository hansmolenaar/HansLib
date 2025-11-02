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
