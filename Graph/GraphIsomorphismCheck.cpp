#include "GraphIsomorphismCheck.h"

using namespace GraphIsomorphism;

Status Check::operator()(const UndirectedGraph &g0, const UndirectedGraph &g1)
{
    if (g0.getNumVertices() != g1.getNumVertices())
    {
        return NotIsomorphic;
    }
    if (g0.getNumEdges() != g1.getNumEdges())
    {
        return NotIsomorphic;
    }

    const auto degreeSequence0 = g0.getSortedDegreeSequence();
    const auto degreeSequence1 = g1.getSortedDegreeSequence();
    if (degreeSequence0 != degreeSequence1)
    {
        return NotIsomorphic;
    }

    std::vector<GraphVertex> adjacent0;
    std::vector<GraphVertex> adjacent1;
    const auto nVertices = g0.getNumVertices();
    for (GraphVertex v = 0; v < nVertices; ++v)
    {
        g0.setAdjacentVertices(v, adjacent0);
        g1.setAdjacentVertices(v, adjacent1);
        if (adjacent0 != adjacent1)
        {
            return Undecided;
        }
    }

    return Isomorphic;
}
