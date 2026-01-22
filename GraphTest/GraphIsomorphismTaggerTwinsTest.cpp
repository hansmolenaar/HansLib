#include <gtest/gtest.h>

#include "Defines.h"
#include "GraphIsomorphismICompareTest.h"
#include "GraphIsomorphismITaggerTest.h"
#include "GraphIsomorphismTaggerTwins.h"
#include "UndirectedGraphFromG6.h"
#include "UndirectedGraphLibrary.h"

using namespace GraphIsomorphism;
using namespace Graph;

TEST(GraphIsomorphismTaggerTwinsTest, CheckTagger)
{
    TaggerTwinsFactory factory;
    GraphTest::CheckTagger(factory);
}

TEST(GraphIsomorphismTaggerTwinsTest, CheckList)
{
    CompareTwinsFactory factory;
    GraphTest::CheckList(factory, UndirectedGraphFromG6::getListNumVertices_5(), Tag{1, 8, 2, 2, 3, 1, 4, 3, 7, 1});
}

TEST(GraphIsomorphismTaggerTwinsTest, Path3)
{
    const auto graph = UndirectedGraphLibrary::Get_Path(3);
    TaggerTwinsFactory factory;
    GraphTest::CheckTaggerConsistency(*graph, factory, 1);

    const TaggerTwins tagger(*graph);
    ASSERT_EQ(tagger.getGraphTag(), (Tag{{0, 1, 2, 2, 0, 3}}));
    ASSERT_EQ(tagger.getVertexTag(0), (Tag{2, 0}));
    ASSERT_EQ(tagger.getVertexTag(1), (Tag{}));
    ASSERT_EQ(tagger.getVertexTag(2), (Tag{2, 0}));

    ASSERT_EQ(tagger.compareOtherGraph(0, tagger, 1), std::weak_ordering::greater);
    ASSERT_EQ(tagger.compareOtherGraph(1, tagger, 0), std::weak_ordering::less);
}

TEST(GraphIsomorphismTaggerTwinsTest, Cycle3)
{
    const auto graph = UndirectedGraphLibrary::Get_Cycle(3);
    TaggerTwinsFactory factory;
    GraphTest::CheckTaggerConsistency(*graph, factory, 0);

    const TaggerTwins tagger(*graph);
    ASSERT_EQ(tagger.getGraphTag(), (Tag{{
                                        0,
                                        3,
                                        3,
                                        3,
                                    }}));
    ASSERT_EQ(tagger.getVertexTag(0), (Tag{0, 3}));
    ASSERT_EQ(tagger.getVertexTag(1), (Tag{0, 3}));
    ASSERT_EQ(tagger.getVertexTag(2), (Tag{0, 3}));

    ASSERT_EQ(tagger.compareOtherGraph(0, tagger, 1), std::weak_ordering::equivalent);
}

TEST(GraphIsomorphismTaggerTwinsTest, Claw)
{
    const auto graph = UndirectedGraphLibrary::Get_Claw();
    TaggerTwinsFactory factory;
    GraphTest::CheckTaggerConsistency(*graph, factory, 1);

    const TaggerTwins tagger(*graph);
    ASSERT_EQ(tagger.getGraphTag(), (Tag{{
                                        0,
                                        1,
                                        3,
                                        3,
                                        0,
                                        4,
                                    }}));
    ASSERT_EQ(tagger.getVertexTag(0), (Tag{}));
    ASSERT_EQ(tagger.getVertexTag(1), (Tag{3, 0}));
    ASSERT_EQ(tagger.getVertexTag(2), (Tag{3, 0}));
    ASSERT_EQ(tagger.getVertexTag(3), (Tag{3, 0}));

    const auto cycle4 = UndirectedGraphLibrary::Get_Cycle(3);
    const TaggerTwins taggerCycle4(*cycle4);
    ASSERT_EQ(tagger.getGraphTag() <=> taggerCycle4.getGraphTag(), std::strong_ordering::less);
    ASSERT_EQ(tagger.compareOtherGraph(0, taggerCycle4, 0), std::weak_ordering::less);
    ASSERT_EQ(taggerCycle4.compareOtherGraph(0, tagger, 0), std::weak_ordering::greater);
}

TEST(GraphIsomorphismTaggerTwinsTest, Disconnected)
{
    const auto graph = UndirectedGraphFromG6::Create("D?K");
    const TaggerTwins tagger(*graph);
    ASSERT_EQ(tagger.getGraphTag(), (Tag{{0, 3, 2, 2, 0, 5}}));
    ASSERT_EQ(tagger.getVertexTag(0), (Tag{}));
    ASSERT_EQ(tagger.getVertexTag(1), (Tag{}));
    ASSERT_EQ(tagger.getVertexTag(2), (Tag{2, 0}));
    ASSERT_EQ(tagger.getVertexTag(3), (Tag{2, 0}));
    ASSERT_EQ(tagger.getVertexTag(4), (Tag{}));
}
