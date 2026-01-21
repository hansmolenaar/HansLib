#include <gtest/gtest.h>

#include "GraphIsomorphismICompareTest.h"

#include "GraphIsomorphismGrouping.h"
#include "GraphIsomorphismUtils.h"
#include "UndirectedGraphFromG6.h"
#include "UndirectedGraphLibrary.h"

using namespace Graph;
using namespace GraphTest;
using namespace GraphIsomorphism;

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

void CheckListGraphCompare(ICompareFactory &factory, const std::vector<const ICompare *> &list,
                           Tag expectGraphTagMultiplicities)
{
    std::vector<const IGraphCompare *> gcomparers(list.size());
    str::transform(list, gcomparers.begin(), [](const auto &c) { return dynamic_cast<const IGraphCompare *>(c); });

    CheckListGraphCompareSymmetry(factory, gcomparers);

    auto cmp = [](const IGraphCompare *p1, const IGraphCompare *p2) {
        return p1->compareOtherGraph(*p2) == std::weak_ordering::less;
    };
    const Grouping<const IGraphCompare *> grouping(gcomparers, cmp);
    const auto tag = CondenseSizeSequence(grouping.getGroupSizes());
    ASSERT_EQ(tag, expectGraphTagMultiplicities);
}

} // namespace

void GraphTest::CheckList(ICompareFactory &factory, const std::vector<std::string> &g6list,
                          Tag expectGraphTagMultiplicities)
{
    const auto graph0 = UndirectedGraphLibrary::Get_Null();
    const auto compare0 = factory.createCompare(*graph0);
    ASSERT_EQ(compare0->getGraph().getNumVertices(), 0);

    const auto graphs = UndirectedGraphFromG6::getGraphs(g6list);
    std::vector<std::unique_ptr<ICompare>> comparers_up;
    str::transform(graphs, std::back_inserter(comparers_up),
                   [&factory](const auto &upg) { return factory.createCompare(*upg); });
    std::vector<const ICompare *> comparers(g6list.size());
    str::transform(comparers_up, comparers.begin(), [](const auto &cmp) { return cmp.get(); });

    if (compare0->getGraphCompare() != nullptr)
    {
        ASSERT_EQ(compare0->getGraphCompare()->compareOtherGraph(*compare0->getGraphCompare()),
                  std::weak_ordering::equivalent);
        CheckListGraphCompare(factory, comparers, expectGraphTagMultiplicities);
    }
    else
    {
        ASSERT_TRUE(expectGraphTagMultiplicities.empty());
    }
}
