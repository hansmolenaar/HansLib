#include <gtest/gtest.h>

#include "Defines.h"
#include "GraphIsomorphismITaggerTest.h"
#include "GraphIsomorphismStatus.h"
#include "GraphIsomorphismTaggerNumbers.h"
#include "UndirectedGraphLibrary.h"

using namespace GraphIsomorphism;

namespace
{
} // namespace

TEST(GraphIsomorphismTaggerNumbersTest, CheckGraphTagger)
{
    TaggerNumbersFactory factory;
    GraphTest::CheckGraphTagger(factory);
}

TEST(GraphIsomorphismTaggerNumbersTest, Path3)
{
    const auto graph = UndirectedGraphLibrary::Get_Path(3);
    TaggerNumbersFactory factory;
    GraphTest::CheckGraphTaggerConsistency(*graph, factory);

    const TaggerNumbers tagger(*graph);
    ASSERT_TRUE(str::equal(tagger.getGraphTag(), Tag{3, 2}));
}

TEST(GraphIsomorphismTaggerNumbersTest, Cycle3)
{
    const auto graph = UndirectedGraphLibrary::Get_Cycle(3);
    TaggerNumbersFactory factory;
    GraphTest::CheckGraphTaggerConsistency(*graph, factory);

    const TaggerNumbers tagger(*graph);
    ASSERT_TRUE(str::equal(tagger.getGraphTag(), Tag{3, 3}));
}
