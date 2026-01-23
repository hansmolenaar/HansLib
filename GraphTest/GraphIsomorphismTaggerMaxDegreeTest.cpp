#include <gtest/gtest.h>

#include "Defines.h"
#include "GraphIsomorphismICompareTest.h"
#include "GraphIsomorphismTaggerMaxDegree.h"
#include "GraphUsc.h"
#include "UndirectedGraphFromG6.h"
#include "UndirectedGraphLibrary.h"

using namespace GraphIsomorphism;
using namespace Graph;

namespace
{
} // namespace

TEST(GraphIsomorphismTaggerMaxDegreeTest, CheckTagger)
{
    CompareMaxDegreeFactory factory;
    GraphTest::CheckComparerBasics(factory, Tag{1, 2, 2, 1, 4, 1});
}

TEST(GraphIsomorphismTaggerMaxDegreeTest, CheckList)
{
    CompareMaxDegreeFactory factory;
    GraphTest::CheckList(factory, UndirectedGraphFromG6::getListNumVertices_5(), Tag{1, 2, 2, 1, 7, 1, 23, 1});
}

TEST(GraphIsomorphismTaggerMaxDegreeTest, Path3)
{
    const auto graph = UndirectedGraphLibrary::Get_Path(3);
    CompareMaxDegreeFactory factory;
    GraphTest::CheckVertexCompareConsistency(*graph, factory, 1);

    const TaggerMaxDegree tagger(*graph);
    ASSERT_EQ(tagger.getGraphTag(), (Tag{1}));
    ASSERT_EQ(tagger.getVertexTag(0), 0);
    ASSERT_EQ(tagger.getVertexTag(1), 1);
    ASSERT_EQ(tagger.getVertexTag(2), 0);
}

TEST(GraphIsomorphismTaggerMaxDegreeTest, Diamond)
{
    const auto graph = UndirectedGraphLibrary::Get_Diamond();
    CompareMaxDegreeFactory factory;
    GraphTest::CheckVertexCompareConsistency(*graph, factory, 2);

    const TaggerMaxDegree tagger(*graph);
    ASSERT_EQ(tagger.getGraphTag(), (Tag{2}));
    ASSERT_EQ(tagger.getVertexTag(0), 0);
    ASSERT_EQ(tagger.getVertexTag(1), 1);
    ASSERT_EQ(tagger.getVertexTag(2), 2);
    ASSERT_EQ(tagger.getVertexTag(3), 0);
}
