#include <gtest/gtest.h>

#include "Defines.h"
#include "GraphIsomorphismICompareTest.h"
#include "GraphIsomorphismStatus.h"
#include "GraphIsomorphismTaggerKnown.h"
#include "UndirectedGraphFromG6.h"
#include "UndirectedGraphLibrary.h"

using namespace GraphIsomorphism;

namespace
{
} // namespace

TEST(GraphIsomorphismTaggerKnownTest, CheckList)
{
    CompareKnownFactory factory;
    GraphTest::CheckList(factory, UndirectedGraphFromG6::getListNumVertices_4(), Tag{1, 4, 7, 1});
}

TEST(GraphIsomorphismTaggerKnownTest, Singleton)
{
    const auto graph = UndirectedGraphLibrary::Get_Singleton();
    const TaggerKnown tagger(*graph);
    const auto tag = tagger.getGraphTag();
    ASSERT_EQ(tag, (Tag{TaggerKnown::KnownType::Complete, 1}));
    ASSERT_EQ(tagger.getDescription(), "complete graph of order 1");
}

TEST(GraphIsomorphismTaggerKnownTest, Path2)
{
    const auto graph = UndirectedGraphLibrary::Get_Path(2);
    const TaggerKnown tagger(*graph);
    const auto tag = tagger.getGraphTag();
    ASSERT_EQ(tag, (Tag{TaggerKnown::KnownType::Complete, 2}));
    ASSERT_EQ(tagger.getDescription(), "complete graph of order 2");
}

TEST(GraphIsomorphismTaggerKnownTest, Path3)
{
    const auto graph = UndirectedGraphLibrary::Get_Path(3);
    const TaggerKnown tagger(*graph);
    const auto tag = tagger.getGraphTag();
    ASSERT_EQ(tag, (Tag{TaggerKnown::KnownType::Path, 3}));
    ASSERT_EQ(tagger.getDescription(), "path of order 3");
}

TEST(GraphIsomorphismTaggerKnownTest, Path4)
{
    const auto graph = UndirectedGraphLibrary::Get_Path(4);
    const TaggerKnown tagger(*graph);
    const auto tag = tagger.getGraphTag();
    ASSERT_EQ(tag, (Tag{TaggerKnown::KnownType::Path, 4}));
    ASSERT_EQ(tagger.getDescription(), "path of order 4");
}

TEST(GraphIsomorphismTaggerKnownTest, Cycle3)
{
    const auto graph = UndirectedGraphLibrary::Get_Cycle(3);
    const TaggerKnown tagger(*graph);
    const auto tag = tagger.getGraphTag();
    ASSERT_EQ(tag, (Tag{TaggerKnown::KnownType::Complete, 3}));
    ASSERT_EQ(tagger.getDescription(), "complete graph of order 3");
}

TEST(GraphIsomorphismTaggerKnownTest, Cycle4)
{
    const auto graph = UndirectedGraphLibrary::Get_Cycle(4);
    const TaggerKnown tagger(*graph);
    const auto tag = tagger.getGraphTag();
    ASSERT_EQ(tag, (Tag{TaggerKnown::KnownType::Cycle, 4}));
    ASSERT_EQ(tagger.getDescription(), "cycle of order 4");
}

TEST(GraphIsomorphismTaggerKnownTest, Diamond)
{
    const auto graph = UndirectedGraphLibrary::Get_Diamond();
    const TaggerKnown tagger(*graph);
    const auto tag = tagger.getGraphTag();
    ASSERT_EQ(tag, (Tag{TaggerKnown::KnownType::Unknown}));
    ASSERT_EQ(tagger.getDescription(), "Unknown");
}

TEST(GraphIsomorphismTaggerKnownTest, TwoTriangles)
{
    const auto graph = UndirectedGraphFromG6::Create("EwCW");
    const TaggerKnown tagger(*graph);
    const auto tag = tagger.getGraphTag();
    ASSERT_EQ(tag, (Tag{TaggerKnown::KnownType::Unknown}));
    ASSERT_EQ(tagger.getDescription(), "Unknown");
}

TEST(GraphIsomorphismTaggerKnownTest, Disconnected5)
{
    const auto graph = UndirectedGraphLibrary::Get_DisconnectedGraph(5);
    const TaggerKnown tagger(*graph);
    const auto tag = tagger.getGraphTag();
    ASSERT_EQ(tag, (Tag{TaggerKnown::KnownType::Singletons, 5}));
    ASSERT_EQ(tagger.getDescription(), "completely disconnected graph of order 5");
}

TEST(GraphIsomorphismTaggerKnownTest, CheckTagger)
{
    CompareKnownFactory factory;
    GraphTest::CheckComparerBasics(factory, Tag{1, 5, 2, 2});
}
