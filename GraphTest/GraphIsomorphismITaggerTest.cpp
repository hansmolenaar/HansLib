#include <gtest/gtest.h>

#include "Defines.h"
#include "GraphIsomorphismGrouper.h"
#include "GraphIsomorphismITaggerTest.h"
#include "GraphIsomorphismTagCompare.h"
#include "Permutation.h"

#include <random>

using namespace GraphTest;
using namespace GraphIsomorphism;

namespace
{
};

void GraphTest::CheckTaggerConsistency(const UndirectedGraph &graph, GraphIsomorphism::ITaggerFactory &factory,
                                       int expectNumAssociatedvertices, int numPermutations)
{
    const auto tagger = factory.create(graph);
    const Grouper grouper(*tagger);
    ASSERT_EQ(grouper.countUnique(), expectNumAssociatedvertices);

    std::random_device rd;
    std::mt19937 g(rd());

    std::vector<Permutation::Entry> permut(graph.getNumVertices());
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
    }
}
