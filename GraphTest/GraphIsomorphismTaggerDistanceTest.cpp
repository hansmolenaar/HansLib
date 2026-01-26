#include <gtest/gtest.h>

#include "Defines.h"
#include "GraphIsomorphismICompareTest.h"
#include "GraphIsomorphismTaggerDistance.h"
#include "GraphIsomorphismVertexComparers.h"
#include "GraphUsc.h"
#include "UndirectedGraphDistance.h"
#include "UndirectedGraphFromG6.h"
#include "UndirectedGraphLibrary.h"
#include "UndirectedGraphTriangles.h"

using namespace GraphIsomorphism;
using namespace Graph;

namespace
{
} // namespace

TEST(GraphIsomorphismTaggerDistanceTest, CheckTagger)
{
    CompareDistanceFactory factory;
    GraphTest::CheckComparerBasics(factory, Tag{1, 8});
}

TEST(GraphIsomorphismTaggerDistanceTest, CheckList)
{
    CompareDistanceFactory factory;
    GraphTest::CheckList(factory, UndirectedGraphFromG6::getListNumVertices_5(), Tag{1, 8, 2, 6, 3, 1, 4, 1, 7, 1});
}

TEST(GraphIsomorphismTaggerDistanceTest, Path3)
{
    const auto graph = UndirectedGraphLibrary::Get_Path(3);
    CompareDistanceFactory factory;
    GraphTest::CheckVertexCompareConsistency(GraphUsc(*graph), factory, 1);

    auto distances = std::make_shared<UndirectedGraphDistance>(GraphUsc(*graph));
    auto triangles = std::make_shared<UndirectedGraphTriangles>(*graph);
    const TaggerDistance tagger(distances, triangles);
    ASSERT_EQ(tagger.compareVertexOtherGraph(0, tagger, 2), std::weak_ordering::equivalent);
    ASSERT_EQ(tagger.compareVertexOtherGraph(2, tagger, 0), std::weak_ordering::equivalent);
    ASSERT_EQ(tagger.compareVertexOtherGraph(1, tagger, 2), std::weak_ordering::less);
    ASSERT_EQ(tagger.compareVertexOtherGraph(2, tagger, 1), std::weak_ordering::greater);
    const auto &grouping = tagger.getVertexGrouping();
    ASSERT_EQ(grouping.size(), 3);
    ASSERT_EQ(grouping.getGroupSizes(), (std::vector<size_t>{1, 2}));

    const auto &graphTag = tagger.getGraphTag();
    ASSERT_EQ(graphTag, (Tag{1, 1, 2, 2}));
}

TEST(GraphIsomorphismTaggerDistanceTest, Star121)
{
    const auto graph = UndirectedGraphLibrary::Get_Star({1, 2, 1});
    CompareDistanceFactory factory;
    GraphTest::CheckVertexCompareConsistency(*graph, factory, 3);

    TaggerDistance tagger(*graph);
    EXPECT_EQ(tagger.compare(0, 1), std::weak_ordering::less);
    EXPECT_EQ(tagger.compare(0, 2), std::weak_ordering::greater);
    EXPECT_EQ(tagger.compare(0, 3), std::weak_ordering::less);
    EXPECT_EQ(tagger.compare(0, 4), std::weak_ordering::less);
    EXPECT_EQ(tagger.compare(1, 2), std::weak_ordering::greater);
    EXPECT_EQ(tagger.compare(1, 3), std::weak_ordering::greater);
    EXPECT_EQ(tagger.compare(1, 4), std::weak_ordering::equivalent);
    EXPECT_EQ(tagger.compare(2, 3), std::weak_ordering::less);
    EXPECT_EQ(tagger.compare(2, 4), std::weak_ordering::less);
    EXPECT_EQ(tagger.compare(3, 4), std::weak_ordering::less);

    const auto graphTag = tagger.getGraphTag();
    ASSERT_EQ(graphTag, (Tag{2, 2, 3, 3}));

    const auto &grouping = tagger.getVertexGrouping();
    auto sizes = grouping.getGroupSizes();
    str::sort(sizes);
    ASSERT_EQ(sizes, (std::vector<size_t>{1, 1, 1, 2}));
}

TEST(GraphIsomorphismTaggerDistanceTest, SpecialCase1)
{
    const auto g0 = UndirectedGraphFromG6::CreateConnected("GRQH}w");
    const auto g1 = UndirectedGraphFromG6::CreateConnected("GEhrS{");

    TaggerDistance tagger0(*g0);
    TaggerDistance tagger1(*g1);

    ASSERT_EQ(tagger0.getGraphTag(), tagger1.getGraphTag());
    const VertexComparers compare0(std::vector<const IVertexCompare *>{&tagger0});
    const VertexComparers compare1(std::vector<const IVertexCompare *>{&tagger1});

    ASSERT_TRUE(compare0 < compare1);
}

TEST(GraphIsomorphismTaggerDistanceTest, NumVertices5)
{
    for (const auto &g6 : UndirectedGraphFromG6::getListNumVertices_5_connected())
    {
        const auto graph = UndirectedGraphFromG6::CreateConnected(g6);
        ASSERT_EQ(graph->getNumVertices(), 5);
        CompareDistanceFactory factory;
        GraphTest::CheckVertexCompareConsistency(*graph, factory);
    }
}

TEST(GraphIsomorphismTaggerDistanceTest, NumVertices6)
{
    for (const auto &g6 : UndirectedGraphFromG6::getListNumVertices_6_connected())
    {
        const auto graph = UndirectedGraphFromG6::CreateConnected(g6);
        ASSERT_EQ(graph->getNumVertices(), 6);
        CompareDistanceFactory factory;
        GraphTest::CheckVertexCompareConsistency(*graph, factory);
    }
}

TEST(GraphIsomorphismTaggerDistanceTest, NumVertices7)
{
    for (const auto &g6 : UndirectedGraphFromG6::getListNumVertices_7_connected())
    {
        const auto graph = UndirectedGraphFromG6::CreateConnected(g6);
        ASSERT_EQ(graph->getNumVertices(), 7);
        CompareDistanceFactory factory;
        GraphTest::CheckVertexCompareConsistency(*graph, factory);
    }
}

TEST(GraphIsomorphismTaggerDistanceTest, Disconnected3)
{
    const auto graph = UndirectedGraphLibrary::Get_DisconnectedGraph(3);
    TaggerDistance tagger(*graph);

    const auto &graphTag = tagger.getGraphTag();
    ASSERT_EQ(graphTag, (Tag{0, 3}));

    const auto &grouping = tagger.getVertexGrouping();
    ASSERT_EQ(grouping.getGroupSizes(), (std::vector<size_t>{3}));
}
