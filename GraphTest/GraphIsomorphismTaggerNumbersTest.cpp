#include <gtest/gtest.h>

#include "Defines.h"
#include "GraphIsomorphismGrouping.h"
#include "GraphIsomorphismITaggerTest.h"
#include "GraphIsomorphismStatus.h"
#include "GraphIsomorphismTaggerNumbers.h"
#include "GraphIsomorphismUtils.h"
#include "UndirectedGraphFromG6.h"
#include "UndirectedGraphLibrary.h"

using namespace Graph;
using namespace GraphIsomorphism;

namespace
{

void TestCompare(const IGraphUs &graph, ICompareFactory &factory, ICompare &compare0)
{
    ASSERT_EQ(compare0.getGraph().getNumVertices(), 0);
    const auto compare = factory.createCompare(graph);
    ASSERT_NE(compare->getGraph().getNumVertices(), 0);
}

void CheckListGraphCompareSymmetry(ICompareFactory &factory, const std::vector<const IGraphCompare *> &list)
{
    // Test symmetry
    const auto graph0 = UndirectedGraphLibrary::Get_Null();
    const auto c0 = factory.createCompare(*graph0);
    const auto &gc0 = *c0->getGraphCompare();
    for (const auto *p : list)
    {
        const auto cmp0 = gc0.compareOtherGraph(*p);
        const auto cmp1 = p->compareOtherGraph(gc0);
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

void CheckList(ICompareFactory &factory, const std::vector<std::string> &g6list, Tag expectGraphTagMultiplicities)
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
}

} // namespace

TEST(GraphIsomorphismTaggerNumbersTest, CheckList)
{
    CompareNumbersFactory factory;
    CheckList(factory, UndirectedGraphFromG6::getListNumVertices_5(), Tag{1, 4, 2, 2, 4, 2, 6, 3});
}

TEST(GraphIsomorphismTaggerNumbersTest, CheckTagger)
{
    TaggerNumbersFactory factory;
    GraphTest::CheckTagger(factory);
}

TEST(GraphIsomorphismTaggerNumbersTest, Path3)
{
    const auto graph = UndirectedGraphLibrary::Get_Path(3);
    TaggerNumbersFactory factory;
    GraphTest::CheckTaggerConsistency(*graph, factory);

    const TaggerNumbers tagger(*graph);
    ASSERT_TRUE(str::equal(tagger.getGraphTag(), Tag{3, 2}));
}

TEST(GraphIsomorphismTaggerNumbersTest, Cycle3)
{
    const auto graph = UndirectedGraphLibrary::Get_Cycle(3);
    TaggerNumbersFactory factory;
    GraphTest::CheckTaggerConsistency(*graph, factory);

    const TaggerNumbers tagger(*graph);
    ASSERT_TRUE(str::equal(tagger.getGraphTag(), Tag{3, 3}));
}
