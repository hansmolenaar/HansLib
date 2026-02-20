#include <gtest/gtest.h>

#include "Defines.h"
#include "GraphIsomorphismICompareTest.h"
#include "GraphIsomorphismTaggerShortestCycle.h"
#include "UndirectedGraphFromG6.h"
#include "UndirectedGraphLibrary.h"

using namespace GraphIsomorphism;

namespace
{
} // namespace

TEST(GraphIsomorphismTaggerShortestCycleTest, CheckTagger)
{
    CompareShortestCycleFactory factory;
    GraphTest::CheckComparerBasics(factory, Tag{1, 2, 2, 2, 3, 1});
}

TEST(GraphIsomorphismTaggerShortestCycleTest, CheckList)
{
    CompareShortestCycleFactory factory;
    GraphTest::CheckList(factory, UndirectedGraphFromG6::getListNumVertices_5_connected(), Tag{1, 5, 3, 3, 7, 1});
}

TEST(GraphIsomorphismTaggerShortestCycleTest, Path3)
{
    const auto graph = UndirectedGraphLibrary::Get_Path(3);
    CompareShortestCycleFactory factory;
    GraphTest::CheckVertexCompareConsistency(*graph, factory);

    const TaggerShortestCycle tagger(*graph);
    ASSERT_TRUE(str::equal(tagger.getGraphTag(), Tag{0, 3}));
}

TEST(GraphIsomorphismTaggerShortestCycleTest, Cycle3and4)
{
    const auto graph3 = UndirectedGraphLibrary::Get_Cycle(3);
    const auto graph4 = UndirectedGraphLibrary::Get_Cycle(4);
    CompareShortestCycleFactory factory;
    GraphTest::CheckVertexCompareConsistency(*graph3, factory);
    GraphTest::CheckVertexCompareConsistency(*graph4, factory);

    for (int n = 0; n < 10; ++n)
    {
        const TaggerShortestCycle t(*graph3);
    }
    const auto tagger3 = TaggerShortestCycle(*graph3);
    const auto tagger4 = TaggerShortestCycle(*graph4);
    ASSERT_EQ(tagger3.getGraphTag(), (Tag{3, 3}));
    ASSERT_EQ(tagger4.getGraphTag(), (Tag{4, 4}));

    const auto vertexGrouping3 = tagger3.getVertexGrouping();
    const auto vertexGrouping4 = tagger4.getVertexGrouping();

    ASSERT_EQ(tagger3.compareVertexOtherGraph(0, tagger3, 1), std::weak_ordering::equivalent);
    ASSERT_EQ(tagger3.compareVertexOtherGraph(0, tagger4, 1), std::weak_ordering::less);
}
