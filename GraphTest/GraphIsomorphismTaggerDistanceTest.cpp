#include <gtest/gtest.h>

#include "Defines.h"
#include "GraphIsomorphismITaggerTest.h"
#include "GraphIsomorphismTaggerDistance.h"
#include "GraphUsc.h"
#include "UndirectedGraphDistance.h"
#include "UndirectedGraphFromG6.h"
#include "UndirectedGraphLibrary.h"

using namespace GraphIsomorphism;
using namespace Graph;

namespace
{
} // namespace

TEST(GraphIsomorphismTaggerDistanceTest, CheckTagger)
{
    TaggerDistanceFactory factory;
    GraphTest::CheckVertexTagger(factory);
}

TEST(GraphIsomorphismTaggerDistanceTest, Path3)
{
    const auto graph = UndirectedGraphLibrary::Get_Path(3);
    TaggerDistanceFactory factory;
    GraphTest::CheckTaggerConsistency(GraphUsc(*graph), factory, 1);

    auto distances = std::make_shared<UndirectedGraphDistance>(GraphUsc(*graph));
    const TaggerDistance tagger(distances);
    ASSERT_TRUE(str::equal(tagger.getVertexTag(0), Tag{1, 1}));
    ASSERT_TRUE(str::equal(tagger.getVertexTag(1), Tag{2}));
    ASSERT_TRUE(str::equal(tagger.getVertexTag(2), Tag{1, 1}));

    const auto &graphTag = tagger.getGraphTag();
    ASSERT_EQ(graphTag, (Tag{1, 1, 2, 2}));
}

TEST(GraphIsomorphismTaggerDistanceTest, Star121)
{
    const auto graph = UndirectedGraphLibrary::Get_Star({1, 2, 1});
    TaggerDistanceFactory factory;
    GraphTest::CheckTaggerConsistency(*graph, factory, 3);

    const auto tagger = factory.createVertexTagger(*graph);
    ASSERT_TRUE(str::equal(tagger->getVertexTag(0), Tag{3, 1}));
    ASSERT_TRUE(str::equal(tagger->getVertexTag(1), Tag{1, 2, 1}));
    ASSERT_TRUE(str::equal(tagger->getVertexTag(2), Tag{2, 2}));
    ASSERT_TRUE(str::equal(tagger->getVertexTag(3), Tag{1, 1, 2}));
    ASSERT_TRUE(str::equal(tagger->getVertexTag(4), Tag{1, 2, 1}));

    const auto graphTag = TaggerDistance(*graph).getGraphTag();
    ASSERT_EQ(graphTag, (Tag{2, 2, 3, 3}));
}

TEST(GraphIsomorphismTaggerDistanceTest, NumVertices5)
{
    for (const auto &g6 : UndirectedGraphFromG6::getListNumVertices_5_connected())
    {
        const auto graph = UndirectedGraphFromG6::CreateConnected(g6);
        ASSERT_EQ(graph->getNumVertices(), 5);
        TaggerDistanceFactory factory;
        GraphTest::CheckTaggerConsistency(*graph, factory);
    }
}

TEST(GraphIsomorphismTaggerDistanceTest, NumVertices6)
{
    for (const auto &g6 : UndirectedGraphFromG6::getListNumVertices_6_connected())
    {
        const auto graph = UndirectedGraphFromG6::CreateConnected(g6);
        ASSERT_EQ(graph->getNumVertices(), 6);
        TaggerDistanceFactory factory;
        GraphTest::CheckTaggerConsistency(*graph, factory);
    }
}

TEST(GraphIsomorphismTaggerDistanceTest, NumVertices7)
{
    for (const auto &g6 : UndirectedGraphFromG6::getListNumVertices_7_connected())
    {
        const auto graph = UndirectedGraphFromG6::CreateConnected(g6);
        ASSERT_EQ(graph->getNumVertices(), 7);
        TaggerDistanceFactory factory;
        GraphTest::CheckTaggerConsistency(*graph, factory);
    }
}
