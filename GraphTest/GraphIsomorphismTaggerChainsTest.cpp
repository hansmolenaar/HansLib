#include <gtest/gtest.h>

#include "Defines.h"
#include "GraphIsomorphismITaggerTest.h"
#include "GraphIsomorphismTaggerChains.h"
#include "UndirectedGraphFromG6.h"
#include "UndirectedGraphLibrary.h"

using namespace GraphIsomorphism;

namespace
{
TaggerChainsFactory factoryChains;
} // namespace

TEST(GraphIsomorphismTaggerChainsTest, PureCycle3)
{
    const auto graph = UndirectedGraphLibrary::Get_Cycle(3);
    GraphTest::CheckTaggerConsistency(*graph, factoryChains, 3);

    const auto tagger = TaggerChains(*graph);
    ASSERT_EQ(tagger.getGraph().getNumVertices(), 3);
    ASSERT_TRUE(str::equal(tagger.getTag(0), VertexTag{0, 3, 1, 0}));
    ASSERT_TRUE(str::equal(tagger.getTag(1), VertexTag{0, 3, 1, 1}));
    ASSERT_TRUE(str::equal(tagger.getTag(2), VertexTag{0, 3, 1, 2}));
}

TEST(GraphIsomorphismTaggerChainsTest, PurePath2)
{
    const auto graph = UndirectedGraphLibrary::Get_Path(2);
    GraphTest::CheckTaggerConsistency(*graph, factoryChains, 2);
    const auto tagger = factoryChains.create(*graph);
    ASSERT_EQ(tagger->getGraph().getNumVertices(), 2);
    ASSERT_TRUE(str::equal(tagger->getTag(0), VertexTag{1, 2, 1, 0}));
    ASSERT_TRUE(str::equal(tagger->getTag(1), VertexTag{1, 2, 1, 1}));
}

TEST(GraphIsomorphismTaggerChainsTest, PurePath3)
{
    const auto graph = UndirectedGraphLibrary::Get_Path(3);
    GraphTest::CheckTaggerConsistency(*graph, factoryChains, 3);
    const auto tagger = TaggerChains(*graph);
    ASSERT_EQ(tagger.getGraph().getNumVertices(), 3);
    ASSERT_TRUE(str::equal(tagger.getTag(0), VertexTag{1, 3, 1, 0}));
    ASSERT_TRUE(str::equal(tagger.getTag(1), VertexTag{1, 3, 1, 1}));
    ASSERT_TRUE(str::equal(tagger.getTag(2), VertexTag{1, 3, 1, 2}));
}

TEST(GraphIsomorphismTaggerChainsTest, Pan3)
{
    const auto graph = UndirectedGraphFromG6::Create(UndirectedGraphFromG6::pan3);
    GraphTest::CheckTaggerConsistency(*graph, factoryChains, 4);
    const auto tagger = TaggerChains(*graph);

    ASSERT_TRUE(str::equal(tagger.getTag(0), VertexTag{2, 3, 1, 1}));
    ASSERT_TRUE(str::equal(tagger.getTag(1), VertexTag{2, 3, 1, 2}));
    ASSERT_TRUE(str::equal(tagger.getTag(2), VertexTag{2, 3, 3, 2}));
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
    ASSERT_TRUE(str::equal(tagger.getTag(3), VertexTag{2, 3, 3, 2}));
}

TEST(GraphIsomorphismTaggerChainsTest, Diamond)
{
    const auto graph = UndirectedGraphLibrary::Get_Diamond();
    GraphTest::CheckTaggerConsistency(*graph, factoryChains, 2);
    const auto tagger = TaggerChains(*graph);
    ASSERT_TRUE(str::equal(tagger.getTag(0), VertexTag{4, 3, 1, 1}));
    ASSERT_TRUE(str::equal(tagger.getTag(1), VertexTag{4, 3, 3}));
    ASSERT_TRUE(str::equal(tagger.getTag(2), VertexTag{4, 3, 3}));
    ASSERT_TRUE(str::equal(tagger.getTag(3), VertexTag{4, 3, 2, 1}));
}

TEST(GraphIsomorphismTaggerChainsTest, Star311)
{
    const auto graph = UndirectedGraphLibrary::Get_Star({3, 1, 1});
    GraphTest::CheckTaggerConsistency(*graph, factoryChains, 6);
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
    GraphTest::CheckTaggerConsistency(*graph, factoryChains, 1);
    const auto tagger = TaggerChains(*graph);
    ASSERT_TRUE(str::equal(tagger.getTag(0), VertexTag{4, 4, 1, 1}));
    ASSERT_TRUE(str::equal(tagger.getTag(1), VertexTag{4, 4, 1, 1}));
    ASSERT_TRUE(str::equal(tagger.getTag(2), VertexTag{4, 3, 1, 1}));
    ASSERT_TRUE(str::equal(tagger.getTag(3), VertexTag{4, 3, 4}));
    ASSERT_TRUE(str::equal(tagger.getTag(4), VertexTag{4, 3, 4}));
}

TEST(GraphIsomorphismTaggerChainsTest, X84)
{
    const auto graph = UndirectedGraphFromG6::Create(UndirectedGraphFromG6::X84);
    GraphTest::CheckTaggerConsistency(*graph, factoryChains, 2);

    const auto tagger = TaggerChains(*graph);
    ASSERT_TRUE(str::equal(tagger.getTag(0), VertexTag{4, 3, 1, 1}));
    ASSERT_TRUE(str::equal(tagger.getTag(1), VertexTag{3, 2, 4, 3, 3}));
    ASSERT_TRUE(str::equal(tagger.getTag(2), VertexTag{4, 3, 2, 1}));
    ASSERT_TRUE(str::equal(tagger.getTag(3), VertexTag{3, 2, 4, 3, 3}));
    ASSERT_TRUE(str::equal(tagger.getTag(4), VertexTag{3, 2, 1, 1}));
    ASSERT_TRUE(str::equal(tagger.getTag(4), VertexTag{3, 2, 1, 1}));
}

TEST(GraphIsomorphismTaggerChainsTest, Fish)
{
    const auto graph = UndirectedGraphFromG6::Create(UndirectedGraphFromG6::fish);
    auto tmp = graph->toString();
    GraphTest::CheckTaggerConsistency(*graph, factoryChains, 6);
}

TEST(GraphIsomorphismTaggerChainsTest, NumVertices5)
{
    GraphTest::TaggerCheckListG6(UndirectedGraphFromG6::getListNumVertices_5(), factoryChains, 9);
}

TEST(GraphIsomorphismTaggerChainsTest, NumVertices6)
{
    GraphTest::TaggerCheckListG6(UndirectedGraphFromG6::getListNumVertices_6(), factoryChains, 22);
}

TEST(GraphIsomorphismTaggerChainsTest, NumVertices7)
{
    GraphTest::TaggerCheckListG6(UndirectedGraphFromG6::getListNumVertices_7(), factoryChains, 30);
}

TEST(GraphIsomorphismTaggerChainsTest, NumVertices8)
{
    GraphTest::TaggerCheckListG6(UndirectedGraphFromG6::getListNumVertices_8(), factoryChains, 27);
}
