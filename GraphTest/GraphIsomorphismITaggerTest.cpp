#include <gtest/gtest.h>

#include "Defines.h"
#include "GraphIsomorphismCheck.h"
#include "GraphIsomorphismDefines.h"
#include "GraphIsomorphismGrouper.h"
#include "GraphIsomorphismITaggerTest.h"
#include "GraphIsomorphismTagCompare.h"
#include "GraphUsc.h"
#include "Permutation.h"
#include "UndirectedGraphFromG6.h"
#include "UndirectedGraphLibrary.h"

using namespace Graph;
using namespace GraphTest;
using namespace GraphIsomorphism;

namespace
{
};

void GraphTest::CheckVertexTaggerConsistency(const IGraphUs &graph, GraphIsomorphism::ITaggerFactory &factory,
                                             int expectNumAssociatedvertices, int numPermutations)
{
    const auto nVertices = graph.getNumVertices();
    const auto gtagger = factory.createTagger(graph);
    const auto *tagger = gtagger->getVertexTagger();
    if (tagger == nullptr)
        return;
    const Grouper grouper(*tagger);
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
        const GraphUsc uscGraphPermuted(graphPermuted);
        const auto gtaggerPermuted = factory.createTagger(uscGraphPermuted);
        const auto *taggerPermuted = gtaggerPermuted->getVertexTagger();
        const Grouper grouperPermuted(*taggerPermuted);
        ASSERT_EQ(grouperPermuted.countUnique(), expectNumAssociatedvertices);

        const auto resultCompare = GraphIsomorphism::TagCompare{}.compare({*tagger, *taggerPermuted});
        ASSERT_EQ(resultCompare.TagCompareStatus, TagCompare::Result::TagStatus::Equivalent);
        if (expectNumAssociatedvertices == nVertices)
        {
            const auto checkIsomorphism = Check{}(graph, resultCompare.VertexPairs, uscGraphPermuted);
            ASSERT_TRUE(checkIsomorphism);
        }
    }
}

void GraphTest::CheckGraphTaggerConsistency(const Graph::IGraphUs &graph, ITaggerFactory &factory, int numPermutations)
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

void GraphTest::TaggerCheckListG6(const std::vector<std::string> &stringsG6, ITaggerFactory &factory,
                                  int expectResolved, int numPermutations)
{
    int numResolved = 0;
    for (const auto &g6 : stringsG6)
    {
        const auto graph = UndirectedGraphFromG6::Create(g6);

        CheckGraphTaggerConsistency(*graph, factory, numPermutations);
        CheckVertexTaggerConsistency(*graph, factory, -1, numPermutations);

        const auto vtagger = factory.createTagger(*graph);
        const auto *tagger = vtagger->getVertexTagger();
        if (tagger == nullptr)
            continue;
        const Grouper grouper(*tagger);
        if (grouper.isResolved())
        {
            numResolved += 1;
        }
        else
        {
            // auto tmp = graph->toString();
        }
    }
    ASSERT_EQ(numResolved, expectResolved);
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
