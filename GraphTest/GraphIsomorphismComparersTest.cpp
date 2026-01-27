#include <gtest/gtest.h>

#include "Defines.h"
#include "GraphIsomorphismComparers.h"
#include "GraphIsomorphismComparersFactory.h"
#include "GraphIsomorphismGrouping.h"
#include "GraphIsomorphismICompareTest.h"
#include "GraphIsomorphismIGraphCompareTest.h"
#include "GraphIsomorphismTaggerChains.h"
#include "GraphIsomorphismTaggerTriangles.h"
#include "GraphIsomorphismUtils.h"
#include "Single.h"
#include "UndirectedGraphFromG6.h"
#include "UndirectedGraphLibrary.h"
#include "UniquePointer.h"

using namespace GraphIsomorphism;
using namespace Graph;
using namespace Utilities;

TEST(GraphIsomorphismComparersTest, CheckBasics)
{
    ComparersFactory factory;
    GraphTest::CheckComparerBasics(factory);
}

TEST(GraphIsomorphismComparersTest, Singleton)
{
    const auto graph = UndirectedGraphLibrary::Get_Singleton();
    const Comparers tg(*graph);
    const auto cmp = tg.compareGraph(tg);
    ASSERT_EQ(cmp, std::weak_ordering::equivalent);
}

TEST(GraphIsomorphismComparersTest, Path2)
{
    const auto graph = UndirectedGraphLibrary::Get_Path(2);
    const auto permuted = UndirectedGraph::CreatePermuted(*graph, Permutation::Create({1, 0}));
    const Comparers tg(*graph);
    const Comparers pg(permuted);
    const auto cmp = tg.compareGraph(pg);
    ASSERT_EQ(cmp, std::weak_ordering::equivalent);
}

TEST(GraphIsomorphismComparersTest, DegreePath2and3)
{
    const auto g0 = UndirectedGraphLibrary::Get_Path(2);
    const auto g1 = UndirectedGraphLibrary::Get_Path(3);
    const Comparers tg0(*g0);
    const Comparers tg1(*g1);
    const auto cmp = tg0.compareGraph(tg1);
    ASSERT_EQ(cmp, std::weak_ordering::less);
}

TEST(GraphIsomorphismComparersTest, DegreePath3)
{
    const auto g0 = UndirectedGraphLibrary::Get_Path(3);
    const auto g1 = UndirectedGraphLibrary::Get_Path(3);
    const Comparers tg0(*g0);
    const Comparers tg1(*g1);
    const auto cmp = tg0.compareGraph(tg1);
    ASSERT_EQ(cmp, std::weak_ordering::equivalent);
}

TEST(GraphIsomorphismComparersTest, DegreeStar123)
{
    const auto g0 = UndirectedGraphLibrary::Get_Star({1, 2, 3});
    const auto g1 = UndirectedGraphLibrary::Get_Star({3, 1, 2});
    const Comparers tg0(*g0);
    const Comparers tg1(*g1);
    const auto cmp = tg0.compareGraph(tg1);
    ASSERT_EQ(cmp, std::weak_ordering::equivalent);
}

TEST(GraphIsomorphismComparersTest, DegreePan3)
{
    const auto g0 = UndirectedGraphFromG6::CreateConnected(UndirectedGraphFromG6::pan3);
    const auto g1 = GraphUsc::CreatePermuted(*g0, {2, 1, 0, 3});
    const Comparers tg0(*g0);
    const Comparers tg1(g1);
    const auto cmp = tg0.compareGraph(tg1);
    ASSERT_EQ(cmp, std::weak_ordering::equivalent);
}

TEST(GraphIsomorphismComparersTest, SpecialCase1)
{
    const auto g0 = UndirectedGraphFromG6::CreateConnected("FMhXw");
    const auto g1 = UndirectedGraphFromG6::CreateConnected("FDxZg");
    const Comparers tg0(*g0);
    const Comparers tg1(*g1);
    const auto cmp = tg0.compareGraph(tg1);
    ASSERT_EQ(cmp, std::weak_ordering::greater);

    const TaggerTriangles taggerTriangles0(std::make_shared<UndirectedGraphTriangles>(*g0));
    const TaggerTriangles taggerTriangles1(std::make_shared<UndirectedGraphTriangles>(*g1));

    ASSERT_EQ(taggerTriangles0.compareVertexOtherGraph(3, taggerTriangles0, 4), std::weak_ordering::equivalent);
    ASSERT_EQ(taggerTriangles1.compareVertexOtherGraph(3, taggerTriangles1, 4), std::weak_ordering::greater);

    const TaggerChains taggerChains0(*g0);
    const TaggerChains taggerChains1(*g1);

    ASSERT_EQ(taggerChains0.getVertexTag(3), (Tag{4, 3}));
    ASSERT_EQ(taggerChains0.getVertexTag(4), (Tag{4, 3}));

    ASSERT_EQ(taggerChains1.getVertexTag(3), (Tag{4, 3}));
    ASSERT_EQ(taggerChains1.getVertexTag(4), (Tag{4, 3}));
}

TEST(GraphIsomorphismComparersTest, SpecialCase2)
{
    const auto g0 = UndirectedGraphFromG6::Create("G}hPW{");
    const auto g1 = UndirectedGraphFromG6::Create("G~`HW{");
    const Comparers tg0(*g0);
    const Comparers tg1(*g1);
    const auto grouping0 = tg0.getVertexGrouping();
    const auto grouping1 = tg1.getVertexGrouping();
    ASSERT_TRUE(grouping0.getUniqueValues().empty());
    ASSERT_TRUE(grouping1.getUniqueValues().empty());

    const auto cmp = tg0.compareGraph(tg1);
    ASSERT_NE(cmp, std::weak_ordering::equivalent);
}

TEST(GraphIsomorphismComparersTest, CheckList345)
{
    ComparersFactory factory;
    auto list = UndirectedGraphFromG6::getListNumVertices_3();
    const auto list4 = UndirectedGraphFromG6::getListNumVertices_4();
    const auto list5 = UndirectedGraphFromG6::getListNumVertices_5();

    list.insert(list.end(), list4.begin(), list4.end());
    list.insert(list.end(), list5.begin(), list5.end());
    GraphTest::CheckList(factory, list, Tag{1, 49});
}

TEST(GraphIsomorphismComparersTest, CheckList6)
{
    ComparersFactory factory;
    GraphTest::CheckList(factory, UndirectedGraphFromG6::getListNumVertices_6(), {1, 155});
}

TEST(GraphIsomorphismComparersTest, CheckList7)
{
    ComparersFactory factory;
    GraphTest::CheckList(factory, UndirectedGraphFromG6::getListNumVertices_7(), {1, 302});
}

TEST(GraphIsomorphismComparersTest, CheckList8)
{
    ComparersFactory factory;
    GraphTest::CheckList(factory, UndirectedGraphFromG6::getListNumVertices_8(), {1, 746});
}

TEST(GGraphIsomorphismComparersTest, CheckList12)
{
    ComparersFactory factory;
    GraphTest::CheckList(factory, UndirectedGraphFromG6::getListNumVertices_12_connected(),
                         {1, 547, 2, 14, 3, 4, 4, 2});
}

TEST(GraphIsomorphismConstructTest, Disconnected5)
{
    ComparersFactory factory;
    const auto graphs = UndirectedGraphFromG6::getDisconnectedGraphs(UndirectedGraphFromG6::getListNumVertices_5());
    GraphTest::CheckList(factory, graphs, {1, 13});
}

TEST(GraphIsomorphismComparersTest, Disconnected6)
{
    ComparersFactory factory;
    const auto graphs = UndirectedGraphFromG6::getDisconnectedGraphs(UndirectedGraphFromG6::getListNumVertices_6());
    GraphTest::CheckList(factory, graphs, {1, 43});
}

TEST(GraphIsomorphismComparersTest, Disconnected7)
{
    ComparersFactory factory;
    const auto graphs = UndirectedGraphFromG6::getDisconnectedGraphs(UndirectedGraphFromG6::getListNumVertices_7());
    GraphTest::CheckList(factory, graphs, {1, 35});
}

TEST(GraphIsomorphismComparersTest, Disconnected8)
{
    ComparersFactory factory;
    const auto graphs = UndirectedGraphFromG6::getDisconnectedGraphs(UndirectedGraphFromG6::getListNumVertices_8());
    GraphTest::CheckList(factory, graphs, {1, 17});
}

TEST(GraphIsomorphismComparersTest, Disconnected9)
{
    ComparersFactory factory;
    const auto graphs = UndirectedGraphFromG6::getDisconnectedGraphs(UndirectedGraphFromG6::getListNumVertices_9());
    GraphTest::CheckList(factory, graphs, {1, 18});
}
