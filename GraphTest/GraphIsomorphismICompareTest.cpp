#include <gtest/gtest.h>

#include "GraphIsomorphismICompareTest.h"

#include "GraphIsomorphismGrouping.h"
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

void CheckGraphCompareSymmetry(const ICharacteristicsCompare &compare0, const ICharacteristicsCompare &compare1)
{
    const auto cmp0 = compare0.compareCharacteristics(compare1);
    const auto cmp1 = compare1.compareCharacteristics(compare0);

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
                              const std::vector<const ICharacteristicsCompare *> &list)
{
    const auto compare = factory.createCompare(graph);
    for (const auto *p : list)
    {
        CheckGraphCompareSymmetry(*p, *compare->getCharacteristicsCompare());
    }
}

void CheckListGraphCompareSymmetry(ICompareFactory &factory, const std::vector<const ICharacteristicsCompare *> &list)
{
    // Test symmetry
    CheckSymmetryAgainstList(factory, *UndirectedGraphLibrary::Get_Null(), list);
    CheckSymmetryAgainstList(factory, *UndirectedGraphLibrary::Get_Singleton(), list);
    CheckSymmetryAgainstList(factory, *UndirectedGraphLibrary::Get_Path(2), list);
    CheckSymmetryAgainstList(factory, *UndirectedGraphLibrary::Get_Cycle(3), list);

    // Check self
    for (const auto *p : list)
    {
        ASSERT_EQ(p->compareCharacteristics(*p), std::weak_ordering::equivalent);
    }
}

void CheckListGraphCompare(ICompareFactory &factory, const std::vector<const ICharacteristicsCompare *> &gcomparers,
                           Tag expectGraphTagMultiplicities)
{
    CheckListGraphCompareSymmetry(factory, gcomparers);

    auto cmp = [](const ICharacteristicsCompare *p1, const ICharacteristicsCompare *p2) {
        return p1->compareCharacteristics(*p2) == std::weak_ordering::less;
    };
    const Grouping<const ICharacteristicsCompare *> grouping(gcomparers, cmp);
    const auto tag = CondenseSizeSequence(grouping.getGroupSizes());
    ASSERT_EQ(tag, expectGraphTagMultiplicities);
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

    if (compare0->getCharacteristicsCompare() != nullptr)
    {
        ASSERT_EQ(compare0->getCharacteristicsCompare()->compareCharacteristics(*compare0->getCharacteristicsCompare()),
                  std::weak_ordering::equivalent);
        const std::vector<const ICharacteristicsCompare *> characteristicsComparers =
            getCastPointers<const ICharacteristicsCompare>(comparers);
        CheckListGraphCompare(factory, characteristicsComparers, expectGraphTagMultiplicities);
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
    const VertexGrouping &grouping = vertexCompare->getVertexGrouping();
    ASSERT_EQ(graph.getNumVertices(), grouping.size());

    if (expectNumUniqueVertices >= 0)
    {
        ASSERT_EQ(grouping.countUnique(), expectNumUniqueVertices);
    }

    auto groupSizes = vertexCompare->getVertexGrouping().getGroupSizes();
    str::sort(groupSizes);
    for (auto n : Iota::GetRange(numPermutations))
    {
        const UndirectedGraph graphPermuted = UndirectedGraph::CreateRandomShuffled(graph, n);
        const auto gcomparerPermuted = factory.createCompare(graphPermuted);
        const auto *vertexComparePermuted = gcomparerPermuted->getVertexCompare();
        const VertexGrouping groupingPermuted(graph.getVertexRange(), VertexLess{*vertexComparePermuted});
        auto groupSizesPermuted = groupingPermuted.getGroupSizes();
        str::sort(groupSizesPermuted);
        ASSERT_EQ(groupSizes, groupSizesPermuted);
        ASSERT_EQ(grouping.countUnique(), groupingPermuted.countUnique());
    }
}

void GraphTest::CheckComparerBasics(GraphIsomorphism::ICompareFactory &factory, Tag expectGraphTagMultiplicities)
{
    // Test some tiny graphs
    auto graphs = UndirectedGraphFromG6::getGraphs(UndirectedGraphFromG6::getListNumVertices_3());
    graphs.emplace_back(UndirectedGraphLibrary::Get_Null());
    graphs.emplace_back(UndirectedGraphLibrary::Get_Singleton());
    graphs.emplace_back(UndirectedGraphLibrary::Get_Path(2));
    graphs.emplace_back(UndirectedGraphLibrary::Get_DisconnectedGraph(2));
    graphs.emplace_back(UndirectedGraphLibrary::Get_Cycle(3));

    CheckList(factory, graphs, expectGraphTagMultiplicities);
}
