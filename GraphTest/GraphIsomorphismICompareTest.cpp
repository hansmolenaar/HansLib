#include <gtest/gtest.h>

#include "GraphIsomorphismICompareTest.h"

#include "GraphIsomorphismGrouping.h"
#include "GraphIsomorphismITaggerTest.h"
#include "GraphIsomorphismUtils.h"
#include "UndirectedGraphFromG6.h"
#include "UndirectedGraphLibrary.h"
#include "UniquePointer.h"

using namespace Graph;
using namespace GraphTest;
using namespace GraphIsomorphism;
using namespace Utilities;

namespace
{

void TestCompare(const IGraphUs &graph, ICompareFactory &factory, ICompare &compare0)
{
    ASSERT_EQ(compare0.getGraph().getNumVertices(), 0);
    const auto compare = factory.createCompare(graph);
    ASSERT_NE(compare->getGraph().getNumVertices(), 0);
}

void CheckGraphCompareSymmetry(const IGraphCompare &compare0, const IGraphCompare &compare1)
{
    const auto cmp0 = compare0.compareOtherGraph(compare1);
    const auto cmp1 = compare1.compareOtherGraph(compare0);

    if (cmp0 == std::weak_ordering::equivalent)
    {
        ASSERT_EQ(cmp1, std::weak_ordering::equivalent);
    }
    else if (cmp0 == std::weak_ordering::less)
    {
        ASSERT_EQ(cmp1, std::weak_ordering::greater);
    }
    else if (cmp0 == std::weak_ordering::greater)
    {
        ASSERT_EQ(cmp1, std::weak_ordering::less);
    }
    else
    {
        ASSERT_TRUE(false);
    }
}

void CheckSymmetryAgainstList(ICompareFactory &factory, const IGraphUs &graph,
                              const std::vector<const IGraphCompare *> &list)
{
    const auto compare = factory.createCompare(graph);
    for (const auto *p : list)
    {
        CheckGraphCompareSymmetry(*p, *compare->getGraphCompare());
    }
}

void CheckListGraphCompareSymmetry(ICompareFactory &factory, const std::vector<const IGraphCompare *> &list)
{
    // Test symmetry
    CheckSymmetryAgainstList(factory, *UndirectedGraphLibrary::Get_Null(), list);
    CheckSymmetryAgainstList(factory, *UndirectedGraphLibrary::Get_Singleton(), list);
    CheckSymmetryAgainstList(factory, *UndirectedGraphLibrary::Get_Path(2), list);
    CheckSymmetryAgainstList(factory, *UndirectedGraphLibrary::Get_Cycle(3), list);

    // Check self
    for (const auto *p : list)
    {
        ASSERT_EQ(p->compareOtherGraph(*p), std::weak_ordering::equivalent);
    }
}

void CheckListGraphCompare(ICompareFactory &factory, const std::vector<const IGraphCompare *> &gcomparers,
                           Tag expectGraphTagMultiplicities)
{
    CheckListGraphCompareSymmetry(factory, gcomparers);

    auto cmp = [](const IGraphCompare *p1, const IGraphCompare *p2) {
        return p1->compareOtherGraph(*p2) == std::weak_ordering::less;
    };
    const Grouping<const IGraphCompare *> grouping(gcomparers, cmp);
    const auto tag = CondenseSizeSequence(grouping.getGroupSizes());
    ASSERT_EQ(tag, expectGraphTagMultiplicities);
}

void CheckListGraphTagger(ICompareFactory &factory, const std::vector<std::unique_ptr<IGraphUs>> &graphs)
{
    for (const auto &graph : graphs)
    {
        GraphTest::CheckGraphTaggerConsistency(*graph, factory);
    }
}

void CheckListVertexCompare(ICompareFactory &factory, const std::vector<std::unique_ptr<IGraphUs>> &graphs)
{
    for (const auto &graph : graphs)
    {
        CheckVertexCompareConsistency(*graph, factory);
    }
}

} // namespace

void GraphTest::CheckList(ICompareFactory &factory, const std::vector<std::unique_ptr<Graph::IGraphUs>> &graphs,
                          Tag expectGraphTagMultiplicities)
{
    const auto graph0 = UndirectedGraphLibrary::Get_Null();
    const auto compare0 = factory.createCompare(*graph0);
    ASSERT_EQ(compare0->getGraph().getNumVertices(), 0);

    std::vector<std::unique_ptr<ICompare>> comparers;
    str::transform(graphs, std::back_inserter(comparers),
                   [&factory](const auto &upg) { return factory.createCompare(*upg); });

    if (compare0->getGraphCompare() != nullptr)
    {
        ASSERT_EQ(compare0->getGraphCompare()->compareOtherGraph(*compare0->getGraphCompare()),
                  std::weak_ordering::equivalent);
        const std::vector<const IGraphCompare *> graphComparers = getCastPointers<const IGraphCompare>(comparers);
        CheckListGraphCompare(factory, graphComparers, expectGraphTagMultiplicities);
        if (compare0->getGraphTagger() != nullptr)
        {
            CheckListGraphTagger(factory, graphs);
        }
    }
    else
    {
        ASSERT_TRUE(expectGraphTagMultiplicities.empty());
    }

    if (compare0->getVertexCompare() != nullptr)
    {
        CheckListVertexCompare(factory, graphs);
    }
}

void GraphTest::CheckList(ICompareFactory &factory, const std::vector<std::string> &g6list,
                          Tag expectGraphTagMultiplicities)
{
    const auto graphs = UndirectedGraphFromG6::getGraphs(g6list);
    CheckList(factory, graphs, expectGraphTagMultiplicities);
}

void GraphTest::CheckVertexCompareConsistency(const IGraphUs &graph, GraphIsomorphism::ICompareFactory &factory,
                                              int expectNumUniqueVertices)
{
    const Permutation::Entry numPermutations = 10;
    const auto comparer = factory.createCompare(graph);
    const auto *vertexCompare = comparer->getVertexCompare();
    if (vertexCompare == nullptr)
    {
        return;
    }
    const Grouping<Vertex> grouping(graph.getVertexRange(), VertexLess{*vertexCompare});

    if (expectNumUniqueVertices >= 0)
    {
        ASSERT_EQ(grouping.countUnique(), expectNumUniqueVertices);
    }

    for (auto n : Iota::GetRange(numPermutations))
    {
        const UndirectedGraph graphPermuted = UndirectedGraph::CreateRandomShuffled(graph, n);
        const auto gcomparerPermuted = factory.createCompare(graphPermuted);
        const auto *vertexComparePermuted = gcomparerPermuted->getVertexCompare();
        const Grouping<Vertex> groupingPermuted(graph.getVertexRange(), VertexLess{*vertexComparePermuted});
        ASSERT_EQ(groupingPermuted.getGroupSizes(), grouping.getGroupSizes());
    }
}

void GraphTest::CheckGraphTaggerConsistency(const IGraphUs &graph, ICompareFactory &factory)
{
    const Permutation::Entry numPermutations = 10;
    const auto comparer = factory.createCompare(graph);
    const auto *graphComparer = comparer->getGraphCompare();

    if (graphComparer == nullptr)
        return;

    const auto *tagger = comparer->getGraphTagger();
    const Tag tag = tagger != nullptr ? tagger->getGraphTag() : Tag{};

    for (auto n : Iota::GetRange(numPermutations))
    {
        const UndirectedGraph graphPermuted = UndirectedGraph::CreateRandomShuffled(graph, n);
        const auto comparerPermuted = factory.createCompare(graphPermuted);
        ASSERT_EQ(graphComparer->compareOtherGraph(*comparerPermuted->getGraphCompare()),
                  std::weak_ordering::equivalent);

        const auto *taggerPermuted = comparerPermuted->getGraphTagger();
        if (taggerPermuted != nullptr)
        {
            ASSERT_EQ(tag, taggerPermuted->getGraphTag());
        }
    }
}
