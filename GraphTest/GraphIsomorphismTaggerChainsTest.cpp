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
    const auto graph = UndirectedGraphFromG6::Create(UndirectedGraphFromG6::pan3);
    const auto tagger = TaggerChains(*graph);

    ASSERT_TRUE(str::equal(tagger.getTag(0), VertexTag{2, 3, 1, 1}));
    ASSERT_TRUE(str::equal(tagger.getTag(1), VertexTag{2, 3, 1, 2}));
    ASSERT_TRUE(str::equal(tagger.getTag(2), VertexTag{2, 3,  3, 2}));
    ASSERT_TRUE(str::equal(tagger.getTag(3), VertexTag{3, 2, 1, 1}));
}

TEST(GraphIsomorphismTaggerChainsTest, Pan3Parmuted)
{
    const auto graph = UndirectedGraphFromG6::Create(UndirectedGraphFromG6::pan3);
    const auto permutation = Permutation::Create(std::vector<Permutation::Entry>{
        2,
        0,
        3,
        1,
    });
    const auto permuted = UndirectedGraph::CreatePermuted(*graph, permutation);

    const auto tagger = TaggerChains(permuted);

    ASSERT_TRUE(str::equal(tagger.getTag(0), VertexTag{2, 3, 1, 1}));
    ASSERT_TRUE(str::equal(tagger.getTag(1), VertexTag{3, 2, 1, 1}));
    ASSERT_TRUE(str::equal(tagger.getTag(2), VertexTag{2, 3, 1, 2}));
    ASSERT_TRUE(str::equal(tagger.getTag(3), VertexTag{2, 3,  3, 2}));
}

TEST(GraphIsomorphismTaggerChainsTest, Diamond)
{
    const auto graph = UndirectedGraphLibrary::Get_Diamond();
    const auto tagger = TaggerChains(*graph);
    ASSERT_TRUE(str::equal(tagger.getTag(0), VertexTag{4, 3, 1, 1}));
    ASSERT_TRUE(str::equal(tagger.getTag(1), VertexTag{4, 3, 3}));
    ASSERT_TRUE(str::equal(tagger.getTag(2), VertexTag{4, 3, 3}));
    ASSERT_TRUE(str::equal(tagger.getTag(3), VertexTag{4, 3, 2, 1}));
}

TEST(GraphIsomorphismTaggerChainsTest, Star311)
{
    const auto graph = UndirectedGraphLibrary::Get_Star({3,1,1});
    const auto tagger = TaggerChains(*graph);
    ASSERT_TRUE(str::equal(tagger.getTag(0), VertexTag{3, 2, 3, 2, 3, 4}));
    ASSERT_TRUE(str::equal(tagger.getTag(1), VertexTag{3, 4, 1, 1}));
    ASSERT_TRUE(str::equal(tagger.getTag(2), VertexTag{3, 4, 1, 2}));
    ASSERT_TRUE(str::equal(tagger.getTag(3), VertexTag{3, 4, 1, 3}));
    ASSERT_TRUE(str::equal(tagger.getTag(4), VertexTag{3, 2, 1, 1}));
    ASSERT_TRUE(str::equal(tagger.getTag(5), VertexTag{3, 2, 2, 1}));
}

TEST(GraphIsomorphismTaggerChainsTest, House)
{
    const auto graph = UndirectedGraphFromG6::Create(UndirectedGraphFromG6::house);
auto tmp = graph->toString();
    const auto tagger = TaggerChains(*graph);
    ASSERT_TRUE(str::equal(tagger.getTag(0), VertexTag{4, 4, 1, 1}));
    ASSERT_TRUE(str::equal(tagger.getTag(1), VertexTag{4, 4, 1, 1}));
    ASSERT_TRUE(str::equal(tagger.getTag(2), VertexTag{4, 3, 1, 1}));
    ASSERT_TRUE(str::equal(tagger.getTag(3), VertexTag{4, 3, 4}));
    ASSERT_TRUE(str::equal(tagger.getTag(4), VertexTag{4, 3, 4}));
}


