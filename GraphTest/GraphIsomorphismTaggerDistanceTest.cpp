#include <gtest/gtest.h>

#include "Defines.h"
#include "GraphIsomorphismTaggerDistance.h"
#include "UndirectedGraphDistance.h"
#include "UndirectedGraphLibrary.h"

using namespace GraphIsomorphism;

namespace
{
} // namespace

TEST(GraphIsomorphismTaggerDistanceTest, Path3)
{
    const auto graph = UndirectedGraphLibrary::Get_Path(3);
    const UndirectedGraphDistance distancer(*graph);
    const auto tagger = TaggerDistance(distancer);
    ASSERT_EQ(tagger.getGraph().getNumVertices(), 3);
    const auto tmp = tagger.getTag(1);
    ASSERT_TRUE(str::equal(tagger.getTag(0), VertexTag{1, 1}));
    ASSERT_TRUE(str::equal(tagger.getTag(1), VertexTag{2}));
    ASSERT_TRUE(str::equal(tagger.getTag(2), VertexTag{1, 1}));
}

TEST(GraphIsomorphismTaggerDistanceTest, Star121)
{
    const auto graph = UndirectedGraphLibrary::Get_Star({1, 2, 1});
    const UndirectedGraphDistance distancer(*graph);
    const auto tagger = TaggerDistance(distancer);
    ASSERT_TRUE(str::equal(tagger.getTag(0), VertexTag{3, 1}));
    ASSERT_TRUE(str::equal(tagger.getTag(1), VertexTag{1, 2, 1}));
    ASSERT_TRUE(str::equal(tagger.getTag(2), VertexTag{2, 2}));
    ASSERT_TRUE(str::equal(tagger.getTag(3), VertexTag{1, 1, 2}));
    ASSERT_TRUE(str::equal(tagger.getTag(4), VertexTag{1, 2, 1}));
}
