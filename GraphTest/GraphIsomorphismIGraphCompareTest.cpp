#include <gtest/gtest.h>

#include "GraphIsomorphismIGraphCompareTest.h"

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

std::vector<std::unique_ptr<IGraphCompare>> CreateComparers(IGraphCompareFactory &factory,
                                                            const std::vector<std::unique_ptr<IGraphUs>> &graphs)
{
    std::vector<std::unique_ptr<IGraphCompare>> result;
    str::transform(graphs, std::back_inserter(result),
                   [&factory](const auto &upg) { return factory.createGraphCompare(*upg); });
    return result;
}

void CheckSelfConsistency(IGraphCompareFactory &factory, const IGraphCompare &compare)
{
    const Permutation::Entry numPermutations = 5;
    const auto &graph = compare.getGraph();
    ASSERT_EQ(compare.compareGraph(compare), std::weak_ordering::equivalent);
    const auto grouping = compare.getVertexGrouping();
    const auto groupSizes = CondenseSizeSequence(grouping.getGroupSizes());
    for (auto n : Iota::GetRange(numPermutations))
    {
        const UndirectedGraph graphPermuted = UndirectedGraph::CreateRandomShuffled(graph, n);
        const auto comparerPermuted = factory.createGraphCompare(graphPermuted);
        ASSERT_EQ(compare.compareGraph(*comparerPermuted), std::weak_ordering::equivalent);
        ASSERT_EQ(comparerPermuted->compareGraph(compare), std::weak_ordering::equivalent);

        const auto groupingPermuted = comparerPermuted->getVertexGrouping();
        ASSERT_EQ(grouping.countUnique(), groupingPermuted.countUnique());

        auto groupSizesPermuted = CondenseSizeSequence(groupingPermuted.getGroupSizes());
        ASSERT_EQ(groupSizes, groupSizesPermuted);
    }
}

void CheckSymmetry(const IGraphCompare &compare1, const IGraphCompare &compare2)
{
    const auto cmp = compare1.compareGraph(compare2);
    if (cmp == std::weak_ordering::equivalent)
    {
        ASSERT_EQ(compare2.compareGraph(compare1), std::weak_ordering::equivalent);
        ASSERT_EQ(compare1.getVertexGrouping()(), compare2.getVertexGrouping()());
        ASSERT_EQ(compare1.getVertexGrouping().countUnique(), compare2.getVertexGrouping().countUnique());
    }
    else if (cmp == std::weak_ordering::less)
    {
        ASSERT_EQ(compare2.compareGraph(compare1), std::weak_ordering::greater);
    }
    else
    {
        ASSERT_EQ(compare2.compareGraph(compare1), std::weak_ordering::less);
    }
}

void CheckConsistencyList(IGraphCompareFactory &factory, const std::vector<const IGraphCompare *> &comparers)
{
    for (const auto *cmp1 : comparers)
    {
        CheckSelfConsistency(factory, *cmp1);
        for (const auto *cmp2 : comparers)
        {
            CheckSymmetry(*cmp1, *cmp2);
        }
    }
}

void CheckGraphGrouping(const std::vector<const IGraphCompare *> &comparers, Tag expectMultiplicities)
{
    const Grouping<const IGraphCompare *> grouping(comparers, [](const IGraphCompare *lhs, const IGraphCompare *rhs) {
        return lhs->compareGraph(*rhs) == std::weak_ordering::less;
    });
    const auto multiplicities = CondenseSizeSequence(grouping.getGroupSizes());
    ASSERT_EQ(multiplicities, expectMultiplicities);
}

} // namespace

void GraphTest::CheckList(IGraphCompareFactory &factory, const std::vector<std::unique_ptr<Graph::IGraphUs>> &graphs,
                          Tag expectMultiplicities)
{
    std::vector<std::unique_ptr<IGraphCompare>> comparers;
    str::transform(graphs, std::back_inserter(comparers),
                   [&factory](const auto &upg) { return factory.createGraphCompare(*upg); });
    CheckGraphGrouping(getCastPointers<const IGraphCompare>(comparers), expectMultiplicities);
}

void GraphTest::CheckList(IGraphCompareFactory &factory, const std::vector<std::string> &g6list,
                          Tag expectGraphTagMultiplicities)
{
    const auto graphs = UndirectedGraphFromG6::getGraphs(g6list);
    CheckList(factory, graphs, expectGraphTagMultiplicities);
}

void GraphTest::CheckComparerBasics(GraphIsomorphism::IGraphCompareFactory &factory)
{
    // Test some tiny graphs
    auto graphs = UndirectedGraphFromG6::getGraphs(UndirectedGraphFromG6::getListNumVertices_3());
    graphs.emplace_back(UndirectedGraphLibrary::Get_Null());
    graphs.emplace_back(UndirectedGraphLibrary::Get_Singleton());
    graphs.emplace_back(UndirectedGraphLibrary::Get_Path(2));
    graphs.emplace_back(UndirectedGraphLibrary::Get_DisconnectedGraph(2));

    const auto comparers = CreateComparers(factory, graphs);

    CheckConsistencyList(factory, getCastPointers<const IGraphCompare>(comparers));
}
