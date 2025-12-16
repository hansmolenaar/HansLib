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
