#include <gtest/gtest.h>

#include "Defines.h"
#include "GraphIsomorphismCheck.h"
#include "GraphIsomorphismDefines.h"
#include "GraphIsomorphismGrouper.h"
#include "GraphIsomorphismITaggerTest.h"
#include "GraphIsomorphismTagCompare.h"
#include "Permutation.h"
#include "UndirectedGraphFromG6.h"

#include <random>

using namespace GraphTest;
using namespace GraphIsomorphism;

namespace
{
};

void GraphTest::CheckTaggerConsistency(const UndirectedGraph &graph, GraphIsomorphism::ITaggerFactory &factory,
                                       int expectNumAssociatedvertices, int numPermutations)
{
    const auto nVertices = graph.getNumVertices();
    const auto tagger = factory.create(graph);
    const Grouper grouper(*tagger);
    if (expectNumAssociatedvertices >= 0)
    {
        ASSERT_EQ(grouper.countUnique(), expectNumAssociatedvertices);
    }
    else
    {
        expectNumAssociatedvertices = grouper.countUnique();
    }

    std::random_device rd;
    std::mt19937 g(rd());
    g.seed(42);

    std::vector<Permutation::Entry> permut(nVertices);
    str::iota(permut, 0);
    for (auto n = 0; n < numPermutations; ++n)
    {
        std::shuffle(permut.begin(), permut.end(), g);
        const auto permutation = Permutation::Create(permut);
        const UndirectedGraph graphPermuted = UndirectedGraph::CreatePermuted(graph, permutation);
        const auto taggerPermuted = factory.create(graphPermuted);
        const Grouper grouperPermuted(*taggerPermuted);
        ASSERT_EQ(grouperPermuted.countUnique(), expectNumAssociatedvertices);

        const auto resultCompare = GraphIsomorphism::TagCompare{}({*tagger, *taggerPermuted});
        ASSERT_EQ(resultCompare.TagCompareStatus, TagCompare::Result::TagStatus::Equivalent);
        if (expectNumAssociatedvertices == nVertices)
        {
            const auto checkIsomorphism = Check{}(graph, resultCompare.VertexPairs, graphPermuted);
            ASSERT_EQ(checkIsomorphism, Status::Isomorphic);
        }
    }
};

void GraphTest::TaggerCheckListG6(const std::vector<std::string> &stringsG6, GraphIsomorphism::ITaggerFactory &factory,
                                  int expectResolved, int numPermutations)
{
    int numResolved = 0;
    for (const auto &g6 : stringsG6)
    {
        const auto graph = UndirectedGraphFromG6::Create(g6);
        GraphTest::CheckTaggerConsistency(*graph, factory, -1, numPermutations);
        const auto tagger = factory.create(*graph);
        const Grouper grouper(*tagger);
        if (grouper.isResolved())
        {
            numResolved += 1;
        }
        else
        {
            //auto tmp = graph->toString();
        }
    }
    ASSERT_EQ(numResolved, expectResolved);
}
