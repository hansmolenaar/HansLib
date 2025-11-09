#include <gtest/gtest.h>

#include "GraphUsc.h"
#include "MyException.h"
#include "UndirectedGraphFromG6.h"
#include "UndirectedGraphLibrary.h"

using namespace Graph;

TEST(GraphUscTest, ConnectedComponents)
{
    const auto graph = UndirectedGraphLibrary::Get_Path(3);
    const GraphUsc uscGraph(*graph);

    ASSERT_EQ(uscGraph.getNumVertices(), 3);
    ASSERT_EQ(uscGraph.getNumEdges(), 2);
    ASSERT_EQ(uscGraph.getDegree(0), 1);
    ASSERT_EQ(uscGraph.getDegree(1), 2);
    ASSERT_EQ(uscGraph.getDegree(2), 1);
    ASSERT_FALSE(uscGraph.areAdjacent(0, 2));
    std::vector<GraphVertex> ngbs;
    uscGraph.setAdjacentVertices(0, ngbs);
    ASSERT_EQ(ngbs, std::vector<GraphVertex>{1});

    const auto components = uscGraph.getConnectedComponents();
    ASSERT_EQ(components, (std::vector<GraphVertex>{0, 0, 0}));
}

TEST(GraphUscTest, Disconnected)
{
    const auto graph = UndirectedGraphFromG6::Create("Eh?G");
    ASSERT_THROW(GraphUsc tmp(*graph), MyException);
}

TEST(GraphUscTest, Permuted)
{
    const auto graph = UndirectedGraphLibrary::Get_Path(3);
    const GraphUsc uscGraph(*graph);
    const GraphUsc permuted = GraphUsc::CreatePermuted(uscGraph, {1, 0, 2});

    ASSERT_EQ(permuted.getNumVertices(), 3);
    ASSERT_EQ(permuted.getNumEdges(), 2);
    ASSERT_EQ(permuted.getDegree(0), 2);
    ASSERT_EQ(permuted.getDegree(1), 1);
    ASSERT_EQ(permuted.getDegree(2), 1);
    ASSERT_FALSE(permuted.areAdjacent(1, 2));
    std::vector<GraphVertex> ngbs;
    permuted.setAdjacentVertices(1, ngbs);
    ASSERT_EQ(ngbs, std::vector<GraphVertex>{0});

    const auto components = permuted.getConnectedComponents();
    ASSERT_EQ(components, (std::vector<GraphVertex>{0, 0, 0}));
}
