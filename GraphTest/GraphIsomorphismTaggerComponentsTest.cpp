#include <gtest/gtest.h>

#include "Defines.h"
#include "GraphIsomorphismITaggerTest.h"
#include "GraphIsomorphismTaggerComponents.h"
#include "UndirectedGraphFromG6.h"
#include "UndirectedGraphLibrary.h"

using namespace GraphIsomorphism;

namespace
{
} // namespace

TEST(GraphIsomorphismTaggerComponentsTest, CheckGraphTagger)
{
    TaggerComponentFactory factory;
    GraphTest::CheckGraphTagger(factory);
}

TEST(GraphIsomorphismTaggerComponentsTest, Path3)
{
    const auto graph = UndirectedGraphLibrary::Get_Path(3);
    const TaggerComponents tagger(*graph);
    ASSERT_TRUE(tagger.isConnected());
    ASSERT_EQ(tagger.getGraphTag(), (Tag{3, 1}));
}

TEST(GraphIsomorphismTaggerComponentsTest, Disconneted)
{
    const auto graph = UndirectedGraphFromG6::Create("Eh?G");
    TaggerComponentFactory factory;
    const auto tagger = factory.createGraphTagger(*graph);
    ASSERT_EQ(tagger->getGraphTag(), (Tag{2, 1, 4, 1}));
}
TEST(GraphIsomorphismTaggerComponentsTest, Disconneted100)
{
    const auto graph = UndirectedGraphLibrary::Get_DisconnectedGraph(100);
    TaggerComponentFactory factory;
    const auto tagger = factory.createGraphTagger(*graph);
    ASSERT_EQ(tagger->getGraphTag(), (Tag{1, 100}));
}
