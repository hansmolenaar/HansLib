#include <gtest/gtest.h>

#include "Defines.h"
#include "GraphIsomorphismITaggerTest.h"
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
    TaggerMaxDegreeFactory factory;
    GraphTest::CheckTagger(factory);
}

TEST(GraphIsomorphismTaggerMaxDegreeTest, Path3)
{
    const auto graph = UndirectedGraphLibrary::Get_Path(3);
    TaggerMaxDegreeFactory factory;
    GraphTest::CheckTaggerConsistency(*graph, factory, 1);

    const TaggerMaxDegree tagger(*graph);
    ASSERT_EQ(tagger.getGraphTag(), (Tag{1}));
    ASSERT_EQ(tagger.getVertexTag(0), 0);
    ASSERT_EQ(tagger.getVertexTag(1), 1);
    ASSERT_EQ(tagger.getVertexTag(2), 0);
}

TEST(GraphIsomorphismTaggerMaxDegreeTest, Diamond)
{
    const auto graph = UndirectedGraphLibrary::Get_Diamond();
    TaggerMaxDegreeFactory factory;
    GraphTest::CheckTaggerConsistency(*graph, factory, 2);

    const TaggerMaxDegree tagger(*graph);
    ASSERT_EQ(tagger.getGraphTag(), (Tag{2}));
    ASSERT_EQ(tagger.getVertexTag(0), 0);
    ASSERT_EQ(tagger.getVertexTag(1), 1);
    ASSERT_EQ(tagger.getVertexTag(2), 2);
    ASSERT_EQ(tagger.getVertexTag(3), 0);
}
