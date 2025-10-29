#include "GraphIsomorphismCheck.h"
#include "Permutation.h"

using namespace GraphIsomorphism;

Status Check::operator()(const UndirectedGraph &g0, const UndirectedGraph &g1) const
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

Status Check::operator()(const UndirectedGraph &g0, const std::vector<VertexPair> &perm01,
                         const UndirectedGraph &g1) const
{
    const auto nVertices = g0.getNumVertices();
    std::vector<Permutation::Entry> perm0(nVertices);
    for (size_t n = 0; n < nVertices; ++n)
    {
        perm0.at(perm01.at(n).first) = perm01.at(n).second;
    }
    const auto g0Permuted = UndirectedGraph::CreatePermuted(g0, Permutation::Create(perm0));

    return Check{}(g0Permuted, g1);
}
