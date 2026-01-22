#include <gtest/gtest.h>

#include "GraphIsomorphismICompareTest.h"
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
CompareTrianglesFactory s_factory;
}

TEST(GraphIsomorphismTaggerTrianglesTest, CheckTagger)
{
    CompareTrianglesFactory factory;
    GraphTest::CheckTagger(factory);
}

TEST(GraphIsomorphismTaggerTrianglesTest, CheckList)
{
    CompareTrianglesFactory factory;
    GraphTest::CheckList(factory, UndirectedGraphFromG6::getListNumVertices_5(), Tag{1, 7, 2, 1, 4, 1, 7, 1, 14, 1});
}

TEST(GraphIsomorphismTaggerTrianglesTest, House)
{
    const auto graph = UndirectedGraphFromG6::CreateConnected(UndirectedGraphFromG6::house);
    const auto comparer = s_factory.createCompare(*graph);
    ASSERT_EQ(comparer->getGraphTagger()->getGraphTag(), (Tag{0, 2, 1, 3}));
}

TEST(GraphIsomorphismTaggerTrianglesTest, Butterfly)
{
    const auto graph = UndirectedGraphFromG6::CreateConnected(UndirectedGraphFromG6::butterfly);
    const auto comparer = s_factory.createCompare(*graph);
    ASSERT_EQ(comparer->getGraphTagger()->getGraphTag(), (Tag{1, 4, 2, 1}));
}

TEST(GraphIsomorphismTaggerTrianglesTest, Diamond)
{
    const auto graph = UndirectedGraphFromG6::CreateConnected(UndirectedGraphFromG6::diamond);
    const auto comparer = s_factory.createCompare(*graph);
    ASSERT_EQ(comparer->getGraphTagger()->getGraphTag(), (Tag{1, 2, 2, 2}));

    const auto *vcompare = comparer->getVertexCompare();
    ASSERT_EQ(vcompare->compareVertexOtherGraph(0, *vcompare, 1), std::weak_ordering::less);
    ASSERT_EQ(vcompare->compareVertexOtherGraph(2, *vcompare, 3), std::weak_ordering::greater);
}
