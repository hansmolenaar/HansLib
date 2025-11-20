#include <gtest/gtest.h>

#include "GraphIsomorphismITaggerTest.h"
#include "GraphIsomorphismTaggerTriangles.h"
#include "Single.h"
#include "UndirectedGraphFromG6.h"
#include "UndirectedGraphLibrary.h"

using namespace GraphIsomorphism;
using namespace Graph;
using namespace Utilities;

TEST(GraphIsomorphismTaggerTrianglesTest, CheckTagger)
{
    TaggerTrianglesFactory factory;
    GraphTest::CheckTagger(factory);
}

TEST(GraphIsomorphismTaggerTrianglesTest, CheckTriangulation)
{
    std::vector<std::array<GraphVertex, 3>> allTriangles;
    std::vector<std::array<GraphVertex, 3>> expect;

    auto graph = UndirectedGraphLibrary::Get_Path(3);
    allTriangles = TaggerTriangles::getAllTriangles(*graph);
    expect = {};
    ASSERT_EQ(allTriangles, expect);

    graph = UndirectedGraphLibrary::Get_Cycle(3);
    allTriangles = TaggerTriangles::getAllTriangles(*graph);
    expect = {std::array<GraphVertex, 3>{0, 1, 2}};
    ASSERT_EQ(allTriangles, expect);

    graph = UndirectedGraphFromG6::CreateConnected(UndirectedGraphFromG6::pan3);
    allTriangles = TaggerTriangles::getAllTriangles(*graph);
    expect = {std::array<GraphVertex, 3>{0, 1, 2}};
    ASSERT_EQ(allTriangles, expect);

    graph = UndirectedGraphFromG6::CreateConnected(UndirectedGraphFromG6::fish);
    allTriangles = TaggerTriangles::getAllTriangles(*graph);
    expect = {std::array<GraphVertex, 3>{3, 4, 5}};
    ASSERT_EQ(allTriangles, expect);
}

TEST(GraphIsomorphismTaggerTrianglesTest, House)
{
    const auto graph = UndirectedGraphFromG6::CreateConnected(UndirectedGraphFromG6::house);
    const auto allTriangles = TaggerTriangles::getAllTriangles(*graph);
    const std::vector<std::array<GraphVertex, 3>> expect = {std::array<GraphVertex, 3>{2, 3, 4}};
    ASSERT_EQ(allTriangles, expect);

    TaggerTrianglesFactory factory;
    const auto tagger = factory.createTagger(*graph);
    ASSERT_EQ(tagger->getGraphTagger()->getGraphTag(), (Tag{0, 2, 1, 3}));
}

TEST(GraphIsomorphismTaggerTrianglesTest, Butterfly)
{
    const auto graph = UndirectedGraphFromG6::CreateConnected(UndirectedGraphFromG6::butterfly);
    const auto allTriangles = TaggerTriangles::getAllTriangles(*graph);
    const std::vector<std::array<GraphVertex, 3>> expect = {
        std::array<GraphVertex, 3>{0, 1, 2},
        std::array<GraphVertex, 3>{0, 3, 4},
    };
    ASSERT_EQ(allTriangles, expect);

    const TaggerTriangles tagger(*graph);
    ASSERT_EQ(tagger.getGraphTag(), (Tag{1, 4, 2, 1}));
}

TEST(GraphIsomorphismTaggerTrianglesTest, Diamond)
{
    const auto graph = UndirectedGraphFromG6::CreateConnected(UndirectedGraphFromG6::diamond);
    const auto allTriangles = TaggerTriangles::getAllTriangles(*graph);
    const std::vector<std::array<GraphVertex, 3>> expect = {
        std::array<GraphVertex, 3>{0, 1, 2},
        std::array<GraphVertex, 3>{1, 2, 3},
    };
    ASSERT_EQ(allTriangles, expect);

    const TaggerTriangles tagger(*graph);
    ASSERT_EQ(tagger.getGraphTag(), (Tag{1, 2, 2, 2}));

    ASSERT_EQ(Single(tagger.getVertexTag(0)), 1);
    ASSERT_EQ(Single(tagger.getVertexTag(1)), 2);
    ASSERT_EQ(Single(tagger.getVertexTag(2)), 2);
    ASSERT_EQ(Single(tagger.getVertexTag(3)), 1);
}

TEST(GraphIsomorphismTaggerTrianglesTest, Complete4)
{
    const auto graph = UndirectedGraphLibrary::Get_CompleteGraph(4);
    const auto allTriangles = TaggerTriangles::getAllTriangles(*graph);
    const std::vector<std::array<GraphVertex, 3>> expect = {
        std::array<GraphVertex, 3>{0, 1, 2},
        std::array<GraphVertex, 3>{0, 1, 3},
        std::array<GraphVertex, 3>{0, 2, 3},
        std::array<GraphVertex, 3>{1, 2, 3},
    };
    ASSERT_EQ(allTriangles, expect);

    const TaggerTriangles tagger(*graph);
    ASSERT_EQ(tagger.getGraphTag(), (Tag{3, 4}));
}

TEST(GraphIsomorphismTaggerTrianglesTest, Complete5)
{
    const auto graph = UndirectedGraphLibrary::Get_CompleteGraph(5);
    const auto allTriangles = TaggerTriangles::getAllTriangles(*graph);
    const std::vector<std::array<GraphVertex, 3>> expect = {
        std::array<GraphVertex, 3>{0, 1, 2}, std::array<GraphVertex, 3>{0, 1, 3}, std::array<GraphVertex, 3>{0, 1, 4},
        std::array<GraphVertex, 3>{0, 2, 3}, std::array<GraphVertex, 3>{0, 2, 4}, std::array<GraphVertex, 3>{0, 3, 4},
        std::array<GraphVertex, 3>{1, 2, 3}, std::array<GraphVertex, 3>{1, 2, 4}, std::array<GraphVertex, 3>{1, 3, 4},
        std::array<GraphVertex, 3>{2, 3, 4},
    };
    ASSERT_EQ(allTriangles, expect);

    const TaggerTriangles tagger(*graph);
    ASSERT_EQ(tagger.getGraphTag(), (Tag{6, 5}));
}
