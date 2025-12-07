#include "GraphIsomorphismCheck.h"
#include "GraphIsomorphismTaggerDegree.h"
#include "GraphIsomorphismTaggerNumbers.h"
#include "Permutation.h"
#include "UndirectedGraph.h"

using namespace Graph;
using namespace GraphIsomorphism;

bool Check::operator()(const IGraphUs &g0, const IGraphUs &g1) const
{
    const TaggerNumbers tagger0(g0);
    const TaggerNumbers tagger1(g1);

    if (tagger0.getGraphTag() != tagger1.getGraphTag())
    {
        return false;
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
            return false;
        }
    }

    return true;
}

bool Check::operator()(const IGraphUs &g0, const std::vector<VertexPair> &perm01, const IGraphUs &g1) const
{
    const auto nVertices = g0.getNumVertices();
    std::vector<Permutation::Entry> perm0(nVertices);
    for (size_t n = 0; n < nVertices; ++n)
    {
        perm0.at(perm01.at(n)[0]) = perm01.at(n)[1];
    }
    const auto g0Permuted = UndirectedGraph::CreatePermuted(g0, Permutation::Create(perm0));

    return Check{}(g0Permuted, g1);
}
