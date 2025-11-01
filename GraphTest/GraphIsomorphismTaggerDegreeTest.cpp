#include <gtest/gtest.h>

#include "Defines.h"
#include "GraphIsomorphismITaggerTest.h"
#include "GraphIsomorphismTaggerDegree.h"
#include "UndirectedGraphLibrary.h"

using namespace GraphIsomorphism;

namespace
{
} // namespace

TEST(GraphIsomorphismTaggerDegreeTest, Path3)
{
    const auto graph = UndirectedGraphLibrary::Get_Path(3);
    TaggerDegreeFactory factory;
    GraphTest::CheckTaggerConsistency(*graph, factory, 1);

    const TaggerDegree tagger(*graph);
    ASSERT_TRUE(str::equal(tagger.getTag(), Tag{1, 1, 2}));
}

// TODO
#if false
TEST(GraphIsomorphismTaggerDegreeTest, Star111)
{
    const auto graph = UndirectedGraphLibrary::Get_Star({1, 1, 1});
    TaggerDegreeFactory factory;
    GraphTest::CheckTaggerConsistency(*graph, factory, 1);

    const auto tagger = TaggerDegree(*graph);
    ASSERT_EQ(tagger.getGraph().getNumVertices(), 4);
    ASSERT_TRUE(str::equal(tagger.getTag(0), Tag{3}));
    ASSERT_TRUE(str::equal(tagger.getTag(1), Tag{1}));
    ASSERT_TRUE(str::equal(tagger.getTag(2), Tag{1}));
    ASSERT_TRUE(str::equal(tagger.getTag(3), Tag{1}));
}
#endif
