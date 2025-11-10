#include <gtest/gtest.h>

#include "Defines.h"
#include "GraphIsomorphismITaggerTest.h"
#include "GraphIsomorphismTaggerChains.h"
#include "GraphUsc.h"
#include "UndirectedGraphFromG6.h"
#include "UndirectedGraphLibrary.h"

using namespace GraphIsomorphism;
using namespace Graph;

namespace
{
TaggerChainsFactory factoryChains;
} // namespace

TEST(GraphIsomorphismTaggerChainsTest, CheckVertexTagger)
{
    GraphTest::CheckVertexTagger(factoryChains);
}

TEST(GraphIsomorphismTaggerChainsTest, PureCycle3)
{
    const GraphUsc graph(*UndirectedGraphLibrary::Get_Cycle(3));
    GraphTest::CheckVertexTaggerConsistency(graph, factoryChains, 3);

    const auto tagger = TaggerChains(GraphUsc(graph));
    ASSERT_TRUE(str::equal(tagger.getVertexTag(0), Tag{0, 3, 1, 0}));
    ASSERT_TRUE(str::equal(tagger.getVertexTag(1), Tag{0, 3, 1, 1}));
    ASSERT_TRUE(str::equal(tagger.getVertexTag(2), Tag{0, 3, 1, 2}));

    ASSERT_EQ(tagger.getGraphTag(), (Tag{0, 3, 1}));
}

TEST(GraphIsomorphismTaggerChainsTest, PurePath2)
{
    const GraphUsc graph(*UndirectedGraphLibrary::Get_Path(2));
    GraphTest::CheckVertexTaggerConsistency(graph, factoryChains, 2);
    const auto gtagger = factoryChains.createTagger(graph);
    const auto *tagger = gtagger->getVertexTagger();
    ASSERT_TRUE(str::equal(tagger->getVertexTag(0), Tag{1, 2, 1, 0}));
    ASSERT_TRUE(str::equal(tagger->getVertexTag(1), Tag{1, 2, 1, 1}));
}

TEST(GraphIsomorphismTaggerChainsTest, PurePath3)
{
    const GraphUsc graph(*UndirectedGraphLibrary::Get_Path(3));
    GraphTest::CheckVertexTaggerConsistency(graph, factoryChains, 3);
    const auto tagger = TaggerChains(graph);
    ASSERT_TRUE(str::equal(tagger.getVertexTag(0), Tag{1, 3, 1, 0}));
    ASSERT_TRUE(str::equal(tagger.getVertexTag(1), Tag{1, 3, 1, 1}));
    ASSERT_TRUE(str::equal(tagger.getVertexTag(2), Tag{1, 3, 1, 2}));

    ASSERT_EQ(tagger.getGraphTag(), (Tag{1, 3, 1}));
}

TEST(GraphIsomorphismTaggerChainsTest, Pan3)
{
    const auto graph = UndirectedGraphFromG6::CreateConnected(UndirectedGraphFromG6::pan3);
    GraphTest::CheckVertexTaggerConsistency(*graph, factoryChains, 4);
    const auto tagger = TaggerChains(*graph);

    ASSERT_TRUE(str::equal(tagger.getVertexTag(0), Tag{2, 3, 1, 1}));
    ASSERT_TRUE(str::equal(tagger.getVertexTag(1), Tag{2, 3, 1, 2}));
    ASSERT_TRUE(str::equal(tagger.getVertexTag(2), Tag{2, 3, 3, 2}));
    ASSERT_TRUE(str::equal(tagger.getVertexTag(3), Tag{3, 2, 1, 1}));

    ASSERT_EQ(tagger.getGraphTag(), (Tag{2, 3, 1, 3, 2, 1}));
}

TEST(GraphIsomorphismTaggerChainsTest, Pan3Parmuted)
{
    const auto graph = UndirectedGraphFromG6::CreateConnected(UndirectedGraphFromG6::pan3);
    const auto permutation = Permutation::Create(std::vector<Permutation::Entry>{
        2,
        0,
        3,
        1,
    });
    const auto permuted = GraphUsc::CreatePermuted(*graph, permutation);
    const auto tagger = TaggerChains(permuted);

    ASSERT_TRUE(str::equal(tagger.getVertexTag(0), Tag{2, 3, 1, 1}));
    ASSERT_TRUE(str::equal(tagger.getVertexTag(1), Tag{3, 2, 1, 1}));
    ASSERT_TRUE(str::equal(tagger.getVertexTag(2), Tag{2, 3, 1, 2}));
    ASSERT_TRUE(str::equal(tagger.getVertexTag(3), Tag{2, 3, 3, 2}));

    ASSERT_EQ(tagger.getGraphTag(), (Tag{2, 3, 1, 3, 2, 1}));
}

TEST(GraphIsomorphismTaggerChainsTest, Diamond)
{
    const GraphUsc graph(*UndirectedGraphLibrary::Get_Diamond());
    GraphTest::CheckVertexTaggerConsistency(graph, factoryChains, 2);
    const TaggerChains tagger(graph);
    ASSERT_TRUE(str::equal(tagger.getVertexTag(0), Tag{4, 3, 1, 1}));
    ASSERT_TRUE(str::equal(tagger.getVertexTag(1), Tag{4, 3, 3}));
    ASSERT_TRUE(str::equal(tagger.getVertexTag(2), Tag{4, 3, 3}));
    ASSERT_TRUE(str::equal(tagger.getVertexTag(3), Tag{4, 3, 2, 1}));

    ASSERT_EQ(tagger.getGraphTag(), (Tag{4, 3, 2}));
}

TEST(GraphIsomorphismTaggerChainsTest, Star311)
{
    const GraphUsc graph(*UndirectedGraphLibrary::Get_Star({3, 1, 1}));
    GraphTest::CheckVertexTaggerConsistency(graph, factoryChains, 6);
    const TaggerChains tagger(graph);
    ASSERT_TRUE(str::equal(tagger.getVertexTag(0), Tag{3, 2, 3, 2, 3, 4}));
    ASSERT_TRUE(str::equal(tagger.getVertexTag(1), Tag{3, 4, 1, 1}));
    ASSERT_TRUE(str::equal(tagger.getVertexTag(2), Tag{3, 4, 1, 2}));
    ASSERT_TRUE(str::equal(tagger.getVertexTag(3), Tag{3, 4, 1, 3}));
    ASSERT_TRUE(str::equal(tagger.getVertexTag(4), Tag{3, 2, 1, 1}));
    ASSERT_TRUE(str::equal(tagger.getVertexTag(5), Tag{3, 2, 2, 1}));

    ASSERT_EQ(tagger.getGraphTag(), (Tag{3, 2, 2, 3, 4, 1}));
}

TEST(GraphIsomorphismTaggerChainsTest, House)
{
    const auto graph = UndirectedGraphFromG6::CreateConnected(UndirectedGraphFromG6::house);
    GraphTest::CheckVertexTaggerConsistency(*graph, factoryChains, 1);
    const auto tagger = TaggerChains(*graph);
    ASSERT_TRUE(str::equal(tagger.getVertexTag(0), Tag{4, 4, 1, 1}));
    ASSERT_TRUE(str::equal(tagger.getVertexTag(1), Tag{4, 4, 1, 1}));
    ASSERT_TRUE(str::equal(tagger.getVertexTag(2), Tag{4, 3, 1, 1}));
    ASSERT_TRUE(str::equal(tagger.getVertexTag(3), Tag{4, 3, 4}));
    ASSERT_TRUE(str::equal(tagger.getVertexTag(4), Tag{4, 3, 4}));

    ASSERT_EQ(tagger.getGraphTag(), (Tag{4, 3, 1, 4, 4, 1}));
}

TEST(GraphIsomorphismTaggerChainsTest, X84)
{
    const auto graph = UndirectedGraphFromG6::CreateConnected(UndirectedGraphFromG6::X84);
    GraphTest::CheckVertexTaggerConsistency(*graph, factoryChains, 2);

    const auto tagger = TaggerChains(*graph);
    ASSERT_TRUE(str::equal(tagger.getVertexTag(0), Tag{4, 3, 1, 1}));
    ASSERT_TRUE(str::equal(tagger.getVertexTag(1), Tag{3, 2, 4, 3, 3}));
    ASSERT_TRUE(str::equal(tagger.getVertexTag(2), Tag{4, 3, 2, 1}));
    ASSERT_TRUE(str::equal(tagger.getVertexTag(3), Tag{3, 2, 4, 3, 3}));
    ASSERT_TRUE(str::equal(tagger.getVertexTag(4), Tag{3, 2, 1, 1}));

    ASSERT_EQ(tagger.getGraphTag(), (Tag{3, 2, 2, 4, 3, 2}));
}

TEST(GraphIsomorphismTaggerChainsTest, Path2andCycle3)
{
    const auto graph = UndirectedGraphFromG6::Create("D`K");
    const auto tagger = TaggerChains(*graph);

    ASSERT_EQ(tagger.getVertexTag(0), (Tag{1, 2, 1, 0}));
    ASSERT_EQ(tagger.getVertexTag(1), (Tag{1, 2, 1, 1}));
    ASSERT_EQ(tagger.getVertexTag(2), (Tag{0, 3, 1, 0}));
    ASSERT_EQ(tagger.getVertexTag(3), (Tag{0, 3, 1, 1}));
    ASSERT_EQ(tagger.getVertexTag(4), (Tag{0, 3, 1, 2}));

    ASSERT_EQ(tagger.getGraphTag(), (Tag{0, 3, 1, 1, 2, 1}));
}

TEST(GraphIsomorphismTaggerChainsTest, Path2_twice)
{
    const auto graph = UndirectedGraphFromG6::Create("CK");
    const auto tagger = TaggerChains(*graph);
    ASSERT_EQ(tagger.getGraphTag(), (Tag{1, 2, 2}));
}

TEST(GraphIsomorphismTaggerChainsTest, Fish)
{
    const auto graph = UndirectedGraphFromG6::CreateConnected(UndirectedGraphFromG6::fish);
    GraphTest::CheckVertexTaggerConsistency(*graph, factoryChains, 6);
}

TEST(GraphIsomorphismTaggerChainsTest, NumVertices5)
{
    GraphTest::TaggerCheckListG6(UndirectedGraphFromG6::getListNumVertices_5_connected(), factoryChains, 9);
}

TEST(GraphIsomorphismTaggerChainsTest, NumVertices6)
{
    GraphTest::TaggerCheckListG6(UndirectedGraphFromG6::getListNumVertices_6_connected(), factoryChains, 22);
}

TEST(GraphIsomorphismTaggerChainsTest, NumVertices7)
{
    GraphTest::TaggerCheckListG6(UndirectedGraphFromG6::getListNumVertices_7_connected(), factoryChains, 30);
}

TEST(GraphIsomorphismTaggerChainsTest, NumVertices8)
{
    GraphTest::TaggerCheckListG6(UndirectedGraphFromG6::getListNumVertices_8_connected(), factoryChains, 27);
}
