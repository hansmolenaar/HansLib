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
    GraphTest::CheckTaggerConsistency(*graph, factory);

    const TaggerNumbers tagger(*graph);
    ASSERT_TRUE(str::equal(tagger.getGraphTag(), Tag{3, 2}));

    const auto status = TaggerNumbers::compare(*graph, *graph);
    ASSERT_EQ(status.getFlag(), Flag::Undecided);
}

TEST(GraphIsomorphismTaggerNumbersTest, Cycle3)
{
    const auto graph = UndirectedGraphLibrary::Get_Cycle(3);
    TaggerNumbersFactory factory;
    GraphTest::CheckTaggerConsistency(*graph, factory);

    const TaggerNumbers tagger(*graph);
    ASSERT_TRUE(str::equal(tagger.getGraphTag(), Tag{3, 3}));

    const auto status = TaggerNumbers::compare(*graph, *graph);
    ASSERT_EQ(status.getFlag(), Flag::Isomorphic);
    ASSERT_EQ(status.getVertexPairs().size(), 3);
}
