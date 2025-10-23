#include <gtest/gtest.h>

#include "Defines.h"
#include "GraphIsomorphismTaggerDegree.h"
#include "UndirectedGraphLibrary.h"

using namespace GraphIsomorphism;

namespace
{
} // namespace

TEST(GraphIsomorphismTaggerDegreeTest, Path3)
{
    const auto graph = UndirectedGraphLibrary::Get_Path(3);
    const auto tagger = TaggerDegree(*graph);
    ASSERT_EQ(tagger.getGraph().getNumVertices(), 3);
    ASSERT_TRUE(str::equal(tagger.getTag(0), VertexTag{1}));
    ASSERT_TRUE(str::equal(tagger.getTag(1), VertexTag{2}));
    ASSERT_TRUE(str::equal(tagger.getTag(2), VertexTag{1}));
}

TEST(GraphIsomorphismTaggerDegreeTest, Star111)
{
    const auto graph = UndirectedGraphLibrary::Get_Star({1, 1, 1});
    const auto tagger = TaggerDegree(*graph);
    ASSERT_EQ(tagger.getGraph().getNumVertices(), 4);
    ASSERT_TRUE(str::equal(tagger.getTag(0), VertexTag{3}));
    ASSERT_TRUE(str::equal(tagger.getTag(1), VertexTag{1}));
    ASSERT_TRUE(str::equal(tagger.getTag(2), VertexTag{1}));
    ASSERT_TRUE(str::equal(tagger.getTag(3), VertexTag{1}));
}
