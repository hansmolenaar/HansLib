#include <gtest/gtest.h>

#include "IGraphUs.h"
#include "UndirectedGraphFromG6.h"
#include "UndirectedGraphLibrary.h"

using namespace Graph;

TEST(IGraphUsTest, isConnected)
{
    ASSERT_TRUE(UndirectedGraphLibrary::Get_Null()->isConnected());
    ASSERT_TRUE(UndirectedGraphLibrary::Get_Singleton()->isConnected());
    ASSERT_TRUE(UndirectedGraphLibrary::Get_Path(2)->isConnected());
    ASSERT_FALSE(UndirectedGraphLibrary::Get_DisconnectedGraph(2)->isConnected());
}

TEST(IGraphUsTest, numComponents)
{
    ASSERT_EQ(UndirectedGraphLibrary::Get_Null()->getNumberOfComponents(), 0);
    ASSERT_EQ(UndirectedGraphLibrary::Get_Singleton()->getNumberOfComponents(), 1);
    ASSERT_EQ(UndirectedGraphLibrary::Get_Path(2)->getNumberOfComponents(), 1);
    ASSERT_EQ(UndirectedGraphLibrary::Get_DisconnectedGraph(2)->getNumberOfComponents(), 2);
}

TEST(IGraphUsTest, isComplete)
{
    ASSERT_TRUE(UndirectedGraphLibrary::Get_Null()->isComplete());
    ASSERT_TRUE(UndirectedGraphLibrary::Get_Singleton()->isComplete());
    ASSERT_TRUE(UndirectedGraphLibrary::Get_Path(2)->isComplete());
    ASSERT_TRUE(UndirectedGraphLibrary::Get_Cycle(3)->isComplete());
    ASSERT_FALSE(UndirectedGraphLibrary::Get_Path(3)->isComplete());
    ASSERT_FALSE(UndirectedGraphLibrary::Get_Cycle(4)->isComplete());
    ASSERT_FALSE(UndirectedGraphLibrary::Get_DisconnectedGraph(2)->isComplete());
}

TEST(IGraphUsTest, isClique)
{
    ASSERT_FALSE(UndirectedGraphLibrary::Get_Null()->isClique(std::vector<Vertex>{}));
    ASSERT_FALSE(UndirectedGraphLibrary::Get_Singleton()->isClique(std::vector<Vertex>{0}));
    ASSERT_FALSE(UndirectedGraphLibrary::Get_Path(2)->isClique(std::vector<Vertex>{ 1}));
    ASSERT_TRUE(UndirectedGraphLibrary::Get_Path(2)->isClique(std::vector<Vertex>{0, 1}));
    ASSERT_TRUE(UndirectedGraphLibrary::Get_Cycle(3)->isClique(std::vector<Vertex>{0, 2}));
    ASSERT_TRUE(UndirectedGraphLibrary::Get_Path(3)->isClique(std::vector<Vertex>{1, 2}));
    ASSERT_FALSE(UndirectedGraphLibrary::Get_Path(3)->isClique(std::vector<Vertex>{0, 2}));
    ASSERT_FALSE(UndirectedGraphLibrary::Get_DisconnectedGraph(5)->isClique(std::vector<Vertex>{0, 2}));
    ASSERT_TRUE(UndirectedGraphLibrary::Get_CompleteGraph(5)->isClique(std::vector<Vertex>{0, 2, 4, 1, 2, 4, 1}));
    ASSERT_TRUE(UndirectedGraphFromG6::Create(UndirectedGraphFromG6::diamond)->isClique(std::vector<Vertex>{1, 2, 3}));
    ASSERT_FALSE(UndirectedGraphFromG6::Create(UndirectedGraphFromG6::diamond)->isClique(std::vector<Vertex>{0, 1, 3}));
}

TEST(IGraphUsTest, FullyConnectedVertices)
{
    auto universalVertices = UndirectedGraphLibrary::Get_Null()->getUniversalVertices();
    ASSERT_TRUE(universalVertices.empty());

    universalVertices = UndirectedGraphLibrary::Get_Singleton()->getUniversalVertices();
    ASSERT_EQ(universalVertices.size(), 1);

    universalVertices = UndirectedGraphLibrary::Get_Path(2)->getUniversalVertices();
    ASSERT_EQ(universalVertices.size(), 2);

    universalVertices = UndirectedGraphLibrary::Get_Path(3)->getUniversalVertices();
    ASSERT_EQ(universalVertices.size(), 1);

    universalVertices = UndirectedGraphLibrary::Get_Cycle(3)->getUniversalVertices();
    ASSERT_EQ(universalVertices.size(), 3);

    universalVertices = UndirectedGraphLibrary::Get_Diamond()->getUniversalVertices();
    ASSERT_EQ(universalVertices.size(), 2);
}

TEST(IGraphUsTest, sortedEdges)
{
    ASSERT_EQ(UndirectedGraphLibrary::Get_Null()->getAllSortedEdges(), (std::vector<std::array<Vertex, 2>>{}));
    ASSERT_EQ(UndirectedGraphLibrary::Get_Singleton()->getAllSortedEdges(), (std::vector<std::array<Vertex, 2>>{}));
    ASSERT_EQ(UndirectedGraphLibrary::Get_Path(2)->getAllSortedEdges(), (std::vector<std::array<Vertex, 2>>{{0, 1}}));
    ASSERT_EQ(UndirectedGraphLibrary::Get_Cycle(3)->getAllSortedEdges(),
              (std::vector<std::array<Vertex, 2>>{{0, 1}, {0, 2}, {1, 2}}));
    ASSERT_EQ(UndirectedGraphLibrary::Get_Path(3)->getAllSortedEdges(),
              (std::vector<std::array<Vertex, 2>>{{0, 1}, {1, 2}}));
    ASSERT_EQ(UndirectedGraphLibrary::Get_DisconnectedGraph(3)->getAllSortedEdges(),
              (std::vector<std::array<Vertex, 2>>{}));
    ASSERT_EQ(UndirectedGraphFromG6 ::Create(UndirectedGraphFromG6::pan3)->getAllSortedEdges(),
              (std::vector<std::array<Vertex, 2>>{{0, 1}, {0, 2}, {1, 2}, {2, 3}}));
    ASSERT_EQ(UndirectedGraphFromG6 ::Create(UndirectedGraphFromG6::diamond)->getAllSortedEdges(),
              (std::vector<std::array<Vertex, 2>>{{0, 1}, {0, 2}, {1, 2}, {1, 3}, {2, 3}}));
}
