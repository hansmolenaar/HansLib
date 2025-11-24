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

TEST(IGraphUsTest, FullyConnectedVertices)
{
    auto fullyConnectedVertices = UndirectedGraphLibrary::Get_Null()->getFullyConnectedVertices();
    ASSERT_TRUE(fullyConnectedVertices.empty());

    fullyConnectedVertices = UndirectedGraphLibrary::Get_Singleton()->getFullyConnectedVertices();
    ASSERT_EQ(fullyConnectedVertices.size(), 1);

    fullyConnectedVertices = UndirectedGraphLibrary::Get_Path(2)->getFullyConnectedVertices();
    ASSERT_EQ(fullyConnectedVertices.size(), 2);

    fullyConnectedVertices = UndirectedGraphLibrary::Get_Path(3)->getFullyConnectedVertices();
    ASSERT_EQ(fullyConnectedVertices.size(), 1);

    fullyConnectedVertices = UndirectedGraphLibrary::Get_Cycle(3)->getFullyConnectedVertices();
    ASSERT_EQ(fullyConnectedVertices.size(), 3);

    fullyConnectedVertices = UndirectedGraphLibrary::Get_Diamond()->getFullyConnectedVertices();
    ASSERT_EQ(fullyConnectedVertices.size(), 2);
}

TEST(IGraphUsTest, sortedEdges)
{
    ASSERT_EQ(UndirectedGraphLibrary::Get_Null()->getAllSortedEdges(), (std::vector<std::array<GraphVertex, 2>>{}));
    ASSERT_EQ(UndirectedGraphLibrary::Get_Singleton()->getAllSortedEdges(),
              (std::vector<std::array<GraphVertex, 2>>{}));
    ASSERT_EQ(UndirectedGraphLibrary::Get_Path(2)->getAllSortedEdges(),
              (std::vector<std::array<GraphVertex, 2>>{{0, 1}}));
    ASSERT_EQ(UndirectedGraphLibrary::Get_Cycle(3)->getAllSortedEdges(),
              (std::vector<std::array<GraphVertex, 2>>{{0, 1}, {0, 2}, {1, 2}}));
    ASSERT_EQ(UndirectedGraphLibrary::Get_Path(3)->getAllSortedEdges(),
              (std::vector<std::array<GraphVertex, 2>>{{0, 1}, {1, 2}}));
    ASSERT_EQ(UndirectedGraphLibrary::Get_DisconnectedGraph(3)->getAllSortedEdges(),
              (std::vector<std::array<GraphVertex, 2>>{}));
    ASSERT_EQ(UndirectedGraphFromG6 ::Create(UndirectedGraphFromG6::pan3)->getAllSortedEdges(),
              (std::vector<std::array<GraphVertex, 2>>{{0, 1}, {0, 2}, {1, 2}, {2, 3}}));
    ASSERT_EQ(UndirectedGraphFromG6 ::Create(UndirectedGraphFromG6::diamond)->getAllSortedEdges(),
              (std::vector<std::array<GraphVertex, 2>>{{0, 1}, {0, 2}, {1, 2}, {1, 3}, {2, 3}}));
}
