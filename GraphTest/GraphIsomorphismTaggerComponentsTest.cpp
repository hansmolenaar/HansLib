#include <gtest/gtest.h>

#include "Defines.h"
#include "GraphIsomorphismICompareTest.h"
#include "GraphIsomorphismITaggerTest.h"
#include "GraphIsomorphismTaggerComponents.h"
#include "UndirectedGraphFromG6.h"
#include "UndirectedGraphLibrary.h"

using namespace GraphIsomorphism;

namespace
{
} // namespace

TEST(GraphIsomorphismTaggerComponentsTest, CheckTagger)
{
    CompareComponentsFactory factory;
    GraphTest::CheckTagger(factory);
}

TEST(GraphIsomorphismTaggerComponentsTest, CheckList)
{
    CompareComponentsFactory factory;
    GraphTest::CheckList(factory, UndirectedGraphFromG6::getListNumVertices_3(), Tag{1, 2, 2, 1});
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
    CompareComponentsFactory factory;
    const auto compare = factory.createCompare(*graph);
    ASSERT_EQ(compare->getGraphTagger()->getGraphTag(), (Tag{2, 1, 4, 1}));
}

TEST(GraphIsomorphismTaggerComponentsTest, Disconneted100)
{
    const auto graph = UndirectedGraphLibrary::Get_DisconnectedGraph(100);
    CompareComponentsFactory factory;
    const auto compare = factory.createCompare(*graph);
    ASSERT_EQ(compare->getGraphTagger()->getGraphTag(), (Tag{1, 100}));
}
