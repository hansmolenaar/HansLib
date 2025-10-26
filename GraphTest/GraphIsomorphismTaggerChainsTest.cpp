#include <gtest/gtest.h>

#include "Defines.h"
#include "GraphIsomorphismTaggerChains.h"
#include "UndirectedGraphFromG6.h"
#include "UndirectedGraphLibrary.h"

using namespace GraphIsomorphism;

namespace
{
} // namespace

TEST(GraphIsomorphismTaggerChainsTest, PureCycle3)
{
    const auto graph = UndirectedGraphLibrary::Get_Cycle(3);
    const auto tagger = TaggerChains(*graph);
    ASSERT_EQ(tagger.getGraph().getNumVertices(), 3);
    ASSERT_TRUE(str::equal(tagger.getTag(0), VertexTag{0, 3, 1, 0}));
    ASSERT_TRUE(str::equal(tagger.getTag(1), VertexTag{0, 3, 1, 1}));
    ASSERT_TRUE(str::equal(tagger.getTag(2), VertexTag{0, 3, 1, 2}));
}

TEST(GraphIsomorphismTaggerChainsTest, PurePath2)
{
    const auto graph = UndirectedGraphLibrary::Get_Path(2);
    const auto tagger = TaggerChains(*graph);
    ASSERT_EQ(tagger.getGraph().getNumVertices(), 2);
    ASSERT_TRUE(str::equal(tagger.getTag(0), VertexTag{1, 2, 1, 0}));
    ASSERT_TRUE(str::equal(tagger.getTag(1), VertexTag{1, 2, 1, 1}));
}

TEST(GraphIsomorphismTaggerChainsTest, PurePath3)
{
    const auto graph = UndirectedGraphLibrary::Get_Path(3);
    const auto tagger = TaggerChains(*graph);
    ASSERT_EQ(tagger.getGraph().getNumVertices(), 3);
    ASSERT_TRUE(str::equal(tagger.getTag(0), VertexTag{1, 3, 1, 0}));
    ASSERT_TRUE(str::equal(tagger.getTag(1), VertexTag{1, 3, 1, 1}));
    ASSERT_TRUE(str::equal(tagger.getTag(2), VertexTag{1, 3, 1, 2}));
}

TEST(GraphIsomorphismTaggerChainsTest, Pan3)
{
    const std::string pan3 = "Cx";
    const auto graph = UndirectedGraphFromG6::Create(pan3);
    const auto tagger = TaggerChains(*graph);

    ASSERT_TRUE(str::equal(tagger.getTag(0), VertexTag{2, 3, 1, 1}));
    ASSERT_TRUE(str::equal(tagger.getTag(1), VertexTag{2, 3, 1, 2}));
    ASSERT_TRUE(str::equal(tagger.getTag(2), VertexTag{2, 3, 3, 2}));
    ASSERT_TRUE(str::equal(tagger.getTag(3), VertexTag{3, 2, 1}));
}

TEST(GraphIsomorphismTaggerChainsTest, Pan3Parmuted)
{
    const std::string pan3 = "Cx";
    const auto graph = UndirectedGraphFromG6::Create(pan3);
    const auto permutation = Permutation::Create(std::vector<Permutation::Entry>{
        2,
        0,
        3,
        1,
    });
    const auto permuted = UndirectedGraph::CreatePermuted(*graph, permutation);

    const auto tagger = TaggerChains(permuted);

    ASSERT_TRUE(str::equal(tagger.getTag(0), VertexTag{2, 3, 1, 1}));
    ASSERT_TRUE(str::equal(tagger.getTag(1), VertexTag{3, 2, 1}));
    ASSERT_TRUE(str::equal(tagger.getTag(2), VertexTag{2, 3, 1, 2}));
    ASSERT_TRUE(str::equal(tagger.getTag(3), VertexTag{2, 3, 3, 2}));
}
