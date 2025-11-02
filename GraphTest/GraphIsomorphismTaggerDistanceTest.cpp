#include <gtest/gtest.h>

#include "Defines.h"
#include "GraphIsomorphismITaggerTest.h"
#include "GraphIsomorphismTaggerDistance.h"
#include "UndirectedGraphDistance.h"
#include "UndirectedGraphFromG6.h"
#include "UndirectedGraphLibrary.h"
#include "UscGraph.h"

using namespace GraphIsomorphism;
using namespace Graph;

namespace
{
} // namespace

TEST(GraphIsomorphismTaggerDistanceTest, Path3)
{
    const auto graph = UndirectedGraphLibrary::Get_Path(3);
    TaggerDistanceFactory factory;
    GraphTest::CheckTaggerConsistency(UscGraph(*graph), factory, 1);

    auto distances = std::make_shared<UndirectedGraphDistance>(UscGraph(*graph));
    const TaggerDistance tagger(distances);
    ASSERT_TRUE(str::equal(tagger.getVertexTag(0), Tag{1, 1}));
    ASSERT_TRUE(str::equal(tagger.getVertexTag(1), Tag{2}));
    ASSERT_TRUE(str::equal(tagger.getVertexTag(2), Tag{1, 1}));
}

TEST(GraphIsomorphismTaggerDistanceTest, Star121)
{
    const UscGraph graph(*UndirectedGraphLibrary::Get_Star({1, 2, 1}));
    TaggerDistanceFactory factory;
    GraphTest::CheckTaggerConsistency(graph, factory, 3);

    const auto tagger = factory.createVertexTagger(graph);
    ASSERT_TRUE(str::equal(tagger->getVertexTag(0), Tag{3, 1}));
    ASSERT_TRUE(str::equal(tagger->getVertexTag(1), Tag{1, 2, 1}));
    ASSERT_TRUE(str::equal(tagger->getVertexTag(2), Tag{2, 2}));
    ASSERT_TRUE(str::equal(tagger->getVertexTag(3), Tag{1, 1, 2}));
    ASSERT_TRUE(str::equal(tagger->getVertexTag(4), Tag{1, 2, 1}));
}

TEST(GraphIsomorphismTaggerDistanceTest, NumVertices5)
{
    for (const auto &g6 : UndirectedGraphFromG6::getListNumVertices_5())
    {
        const auto graph = UndirectedGraphFromG6::CreateConnected(g6);
        ASSERT_EQ(graph->getNumVertices(), 5);
        TaggerDistanceFactory factory;
        GraphTest::CheckTaggerConsistency(*graph, factory);
    }
}

TEST(GraphIsomorphismTaggerDistanceTest, NumVertices6)
{
    for (const auto &g6 : UndirectedGraphFromG6::getListNumVertices_6())
    {
        const auto graph = UndirectedGraphFromG6::CreateConnected(g6);
        ASSERT_EQ(graph->getNumVertices(), 6);
        TaggerDistanceFactory factory;
        GraphTest::CheckTaggerConsistency(*graph, factory);
    }
}

TEST(GraphIsomorphismTaggerDistanceTest, NumVertices7)
{
    for (const auto &g6 : UndirectedGraphFromG6::getListNumVertices_7())
    {
        const auto graph = UndirectedGraphFromG6::CreateConnected(g6);
        ASSERT_EQ(graph->getNumVertices(), 7);
        TaggerDistanceFactory factory;
        GraphTest::CheckTaggerConsistency(*graph, factory);
    }
}
