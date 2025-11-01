#include <gtest/gtest.h>

#include "Defines.h"
#include "GraphIsomorphismITaggerTest.h"
#include "GraphIsomorphismTaggerNumbers.h"
#include "UndirectedGraphLibrary.h"

using namespace GraphIsomorphism;

namespace
{
} // namespace

TEST(GraphIsomorphismTaggerNumbersTest, Path3)
{
    const auto graph = UndirectedGraphLibrary::Get_Path(3);
    TaggerNumbersFactory factory;
    GraphTest::CheckTaggerConsistency(*graph, factory);

    const TaggerNumbers tagger(*graph);
    ASSERT_TRUE(str::equal(tagger.getTag(), Tag{3, 2}));

    const auto status = TaggerNumbers::compare(*graph, *graph);
    ASSERT_EQ(status.StatusFlag, Flag::Undecided);
}

TEST(GraphIsomorphismTaggerNumbersTest, Cycle3)
{
    const auto graph = UndirectedGraphLibrary::Get_Cycle(3);
    TaggerNumbersFactory factory;
    GraphTest::CheckTaggerConsistency(*graph, factory);

    const TaggerNumbers tagger(*graph);
    ASSERT_TRUE(str::equal(tagger.getTag(), Tag{3, 3}));

    const auto status = TaggerNumbers::compare(*graph, *graph);
    ASSERT_EQ(status.StatusFlag, Flag::Isomorphic);
    ASSERT_EQ(status.VertexPairs.size(), 3);
}
