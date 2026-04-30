#include <gtest/gtest.h>

#include "Defines.h"
#include "GraphIsomorphismGrouping.h"
#include "GraphIsomorphismICompareTest.h"
#include "GraphIsomorphismStatus.h"
#include "GraphIsomorphismTaggerNumbers.h"
#include "GraphIsomorphismUtils.h"
#include "UndirectedGraphFromG6.h"
#include "UndirectedGraphLibrary.h"

using namespace Graph;
using namespace GraphIsomorphism;

TEST(GraphIsomorphismTaggerNumbersTest, CheckList)
{
    CompareNumbersFactory factory;
    GraphTest::CheckList(factory, UndirectedGraphFromG6::getListNumVertices_5(), Tag{1, 4, 2, 2, 4, 2, 6, 3});
}

TEST(GraphIsomorphismTaggerNumbersTest, CheckTagger)
{
    CompareNumbersFactory factory;
    GraphTest::CheckComparerBasics(factory, Tag{1, 7, 2, 1});
}

TEST(GraphIsomorphismTaggerNumbersTest, Path3)
{
    const auto graph = UndirectedGraphLibrary::Get_Path(3);
    CompareNumbersFactory factory;
    GraphTest::CheckVertexCompareConsistency(*graph, factory);

    const TaggerNumbers tagger(*graph);
    ASSERT_TRUE(str::equal(tagger.getGraphTag(), Tag{3, 2}));
}

TEST(GraphIsomorphismTaggerNumbersTest, Cycle3)
{
    const auto graph = UndirectedGraphLibrary::Get_Cycle(3);
    CompareNumbersFactory factory;
    GraphTest::CheckVertexCompareConsistency(*graph, factory);

    const TaggerNumbers tagger(*graph);
    ASSERT_TRUE(str::equal(tagger.getGraphTag(), Tag{3, 3}));
}
