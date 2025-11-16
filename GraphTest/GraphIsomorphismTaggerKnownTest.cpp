#include <gtest/gtest.h>

#include "Defines.h"
#include "GraphIsomorphismITaggerTest.h"
#include "GraphIsomorphismStatus.h"
#include "GraphIsomorphismTaggerKnown.h"
#include "UndirectedGraphLibrary.h"

using namespace GraphIsomorphism;

namespace
{
} // namespace

TEST(GraphIsomorphismTaggerKnownTest, Singleton)
{
    const auto graph = UndirectedGraphLibrary::Get_Singleton();
    const TaggerKnown tagger(*graph);
    const auto tag = tagger.getGraphTag();
    ASSERT_EQ(tag, (Tag{TaggerKnown::KnownType::Complete, 1}));
}

TEST(GraphIsomorphismTaggerKnownTest, Path2)
{
    const auto graph = UndirectedGraphLibrary::Get_Path(2);
    const TaggerKnown tagger(*graph);
    const auto tag = tagger.getGraphTag();
    ASSERT_EQ(tag, (Tag{TaggerKnown::KnownType::Complete, 2}));
}

TEST(GraphIsomorphismTaggerKnownTest, Path3)
{
    const auto graph = UndirectedGraphLibrary::Get_Path(3);
    const TaggerKnown tagger(*graph);
    const auto tag = tagger.getGraphTag();
    ASSERT_EQ(tag, (Tag{TaggerKnown::KnownType::Path, 3}));
}

TEST(GraphIsomorphismTaggerKnownTest, Path4)
{
    const auto graph = UndirectedGraphLibrary::Get_Path(4);
    const TaggerKnown tagger(*graph);
    const auto tag = tagger.getGraphTag();
    ASSERT_EQ(tag, (Tag{TaggerKnown::KnownType::Path, 4}));
}

TEST(GraphIsomorphismTaggerKnownTest, Cycle3)
{
    const auto graph = UndirectedGraphLibrary::Get_Cycle(3);
    const TaggerKnown tagger(*graph);
    const auto tag = tagger.getGraphTag();
    ASSERT_EQ(tag, (Tag{TaggerKnown::KnownType::Complete, 3}));
}

TEST(GraphIsomorphismTaggerKnownTest, Cycle4)
{
    const auto graph = UndirectedGraphLibrary::Get_Cycle(4);
    const TaggerKnown tagger(*graph);
    const auto tag = tagger.getGraphTag();
    ASSERT_EQ(tag, (Tag{TaggerKnown::KnownType::Cycle, 4}));
}

TEST(GraphIsomorphismTaggerKnownTest, Diamond)
{
    const auto graph = UndirectedGraphLibrary::Get_Diamond();
    const TaggerKnown tagger(*graph);
    const auto tag = tagger.getGraphTag();
    ASSERT_EQ(tag, (Tag{TaggerKnown::KnownType::Unknown}));
}

TEST(GraphIsomorphismTaggerKnownTest, Disconnected3)
{
    const auto graph = UndirectedGraphLibrary::Get_DisconnectedGraph(5);
    const TaggerKnown tagger(*graph);
    const auto tag = tagger.getGraphTag();
    ASSERT_EQ(tag, (Tag{TaggerKnown::KnownType::Singletons, 5}));
}

TEST(GraphIsomorphismTaggerKnownTest, CheckTagger)
{
    TaggerKnownFactory factory;
    GraphTest::CheckTagger(factory);
}
