#include <gtest/gtest.h>

#include "GraphIsomorphismITaggerTest.h"
#include "GraphIsomorphismTaggerTriangles.h"
#include "Single.h"
#include "UndirectedGraphFromG6.h"
#include "UndirectedGraphLibrary.h"

using namespace GraphIsomorphism;
using namespace Graph;
using namespace Utilities;

namespace
{
TaggerTrianglesFactory s_factory;
}

TEST(GraphIsomorphismTaggerTrianglesTest, CheckTagger)
{
    TaggerTrianglesFactory factory;
    GraphTest::CheckTagger(factory);
}

TEST(GraphIsomorphismTaggerTrianglesTest, House)
{
    const auto graph = UndirectedGraphFromG6::CreateConnected(UndirectedGraphFromG6::house);
    const auto tagger = s_factory.createTagger(*graph);
    ASSERT_EQ(tagger->getGraphTagger()->getGraphTag(), (Tag{0, 2, 1, 3}));
}

TEST(GraphIsomorphismTaggerTrianglesTest, Butterfly)
{
    const auto graph = UndirectedGraphFromG6::CreateConnected(UndirectedGraphFromG6::butterfly);
    const auto tagger = s_factory.createTagger(*graph);
    ASSERT_EQ(tagger->getGraphTagger()->getGraphTag(), (Tag{1, 4, 2, 1}));
}

TEST(GraphIsomorphismTaggerTrianglesTest, Diamond)
{
    const auto graph = UndirectedGraphFromG6::CreateConnected(UndirectedGraphFromG6::diamond);
    const auto tagger = s_factory.createTagger(*graph);
    ASSERT_EQ(tagger->getGraphTagger()->getGraphTag(), (Tag{1, 2, 2, 2}));

    const auto *vcompare = tagger->getVertexCompare();
    ASSERT_EQ(vcompare->compareOtherGraph(0, *vcompare, 1), std::weak_ordering::less);
    ASSERT_EQ(vcompare->compareOtherGraph(2, *vcompare, 3), std::weak_ordering::greater);
}
#if 0
TEST(GraphIsomorphismTaggerTrianglesTest, Complete4)
{
    const auto graph = UndirectedGraphLibrary::Get_CompleteGraph(4);
    const auto allTriangles = TaggerTriangles::getAllTriangles(*graph);
    const std::vector<std::array<Vertex, 3>> expect = {
        std::array<Vertex, 3>{0, 1, 2},
        std::array<Vertex, 3>{0, 1, 3},
        std::array<Vertex, 3>{0, 2, 3},
        std::array<Vertex, 3>{1, 2, 3},
    };
    ASSERT_EQ(allTriangles, expect);

    const TaggerTriangles tagger(*graph);
    ASSERT_EQ(tagger.getGraphTag(), (Tag{3, 4}));
}

TEST(GraphIsomorphismTaggerTrianglesTest, Complete5)
{
    const auto graph = UndirectedGraphLibrary::Get_CompleteGraph(5);
    const auto allTriangles = TaggerTriangles::getAllTriangles(*graph);
    const std::vector<std::array<Vertex, 3>> expect = {
        std::array<Vertex, 3>{0, 1, 2}, std::array<Vertex, 3>{0, 1, 3}, std::array<Vertex, 3>{0, 1, 4},
        std::array<Vertex, 3>{0, 2, 3}, std::array<Vertex, 3>{0, 2, 4}, std::array<Vertex, 3>{0, 3, 4},
        std::array<Vertex, 3>{1, 2, 3}, std::array<Vertex, 3>{1, 2, 4}, std::array<Vertex, 3>{1, 3, 4},
        std::array<Vertex, 3>{2, 3, 4},
    };
    ASSERT_EQ(allTriangles, expect);

    const TaggerTriangles tagger(*graph);
    ASSERT_EQ(tagger.getGraphTag(), (Tag{6, 5}));
}
#endif
