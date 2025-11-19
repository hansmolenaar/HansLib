#include <gtest/gtest.h>

#include "Defines.h"
#include "GraphIsomorphismCheck.h"
#include "GraphIsomorphismDefines.h"
#include "GraphIsomorphismITaggerTest.h"
#include "GraphIsomorphismTagCompare.h"
#include "GraphIsomorphismVertexGrouper.h"
#include "GraphUsc.h"
#include "Permutation.h"
#include "UndirectedGraphFromG6.h"
#include "UndirectedGraphLibrary.h"

using namespace Graph;
using namespace GraphTest;
using namespace GraphIsomorphism;

namespace
{
const int numPermutations = 10;

void CheckVertexTaggerConsistency(const IGraphUs &graph, GraphIsomorphism::ITaggerFactory &factory,
                                  int expectNumAssociatedvertices)
{
    const auto nVertices = graph.getNumVertices();
    const auto gtagger = factory.createTagger(graph);
    const auto *tagger = gtagger->getVertexTagger();
    if (tagger == nullptr)
        return;
    const VertexGrouper grouper(*tagger);
    if (expectNumAssociatedvertices >= 0)
    {
        ASSERT_EQ(grouper.countUnique(), expectNumAssociatedvertices);
    }
    else
    {
        expectNumAssociatedvertices = grouper.countUnique();
    }

    const Permutation trivial = Permutation::CreateTrivial(nVertices);
    for (auto n = 0; n < numPermutations; ++n)
    {
        const auto permutation = Permutation::CreateRandomShuffle(trivial, n);
        const UndirectedGraph graphPermuted = UndirectedGraph::CreatePermuted(graph, permutation);
        const auto gtaggerPermuted = factory.createTagger(graphPermuted);
        const auto *taggerPermuted = gtaggerPermuted->getVertexTagger();
        const VertexGrouper grouperPermuted(*taggerPermuted);
        ASSERT_EQ(grouperPermuted.countUnique(), expectNumAssociatedvertices);

        const auto resultCompare = GraphIsomorphism::TagCompare{}.compare({*tagger, *taggerPermuted});
        ASSERT_EQ(resultCompare.TagCompareStatus, TagCompare::Result::TagStatus::Equivalent);
        if (expectNumAssociatedvertices == nVertices)
        {
            const auto checkIsomorphism = Check{}(graph, resultCompare.VertexPairs, graphPermuted);
            ASSERT_TRUE(checkIsomorphism);
        }
    }
}

void CheckGraphTaggerConsistency(const Graph::IGraphUs &graph, ITaggerFactory &factory)
{
    const auto nVertices = graph.getNumVertices();
    const auto gtagger = factory.createTagger(graph);
    const auto *tagger = gtagger->getGraphTagger();
    if (tagger == nullptr)
        return;
    const auto &tag = tagger->getGraphTag();

    const Permutation trivial = Permutation::CreateTrivial(nVertices);
    for (auto n = 0; n < numPermutations; ++n)
    {
        const auto permutation = Permutation::CreateRandomShuffle(trivial, n);
        const UndirectedGraph graphPermuted = UndirectedGraph::CreatePermuted(graph, permutation);
        const auto gtaggerPermuted = factory.createTagger(graphPermuted);
        const auto *taggerPermuted = gtaggerPermuted->getGraphTagger();
        ASSERT_EQ(tag, taggerPermuted->getGraphTag());
    }
}
}; // namespace

void GraphTest::CheckTaggerConsistency(const IGraphUs &graph, GraphIsomorphism::ITaggerFactory &factory,
                                       int expectNumAssociatedvertices)
{
    CheckGraphTaggerConsistency(graph, factory);
    CheckVertexTaggerConsistency(graph, factory, expectNumAssociatedvertices);
}

namespace
{

void CheckTaggerBasics(GraphIsomorphism::ITaggerFactory &factory, const IGraphUs &graph)
{
    const auto tagger = factory.createTagger(graph);
    Tag tag;
    const auto *graphChecker = tagger->getGraphTagger();

    if (graphChecker != nullptr)
    {
        EXPECT_NO_THROW(tag = graphChecker->getGraphTag());
    }

    const auto *vertexChecker = tagger->getVertexTagger();
    if (vertexChecker != nullptr)
    {
        const auto nVertices = graph.getNumVertices();
        ASSERT_EQ(vertexChecker->getNumVertices(), nVertices);
        for (GraphVertex v = 0; v < nVertices; ++v)
        {
            EXPECT_NO_THROW(tag = vertexChecker->getVertexTag(v));
        }
    }

    CheckTaggerConsistency(graph, factory);
}

}; // namespace

void GraphTest::CheckTagger(ITaggerFactory &factory)
{
    // Test some tiny graphs
    auto graphs = UndirectedGraphFromG6::getGraphs(UndirectedGraphFromG6::getListNumVertices_3());
    graphs.emplace_back(UndirectedGraphLibrary::Get_Null());
    graphs.emplace_back(UndirectedGraphLibrary::Get_Singleton());

    for (const auto &g : graphs)
    {
        CheckTaggerBasics(factory, *g);
    }
}
