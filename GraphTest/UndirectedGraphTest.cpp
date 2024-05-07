#include <gtest/gtest.h>

#include "UndirectedGraphLibrary.h"
#include "Defines.h"
#include "Single.h"


using namespace Utilities;

TEST(UndirectedGraphTest, ConnectedComponents)
{
   UndirectedGraph ug(3);

   ASSERT_EQ(ug.getNumVertices(), 3);
   ASSERT_EQ(ug.getNumEdges(), 0);

   constexpr GraphVertex vertex2 = 2;
   ug.addEdge(1, vertex2);
   ASSERT_EQ(ug.getNumEdges(), 1);

   const auto components = ug.getConnectedComponents();
   ASSERT_EQ(*str::max_element(components), 1);
   ASSERT_EQ(components[vertex2], components[1]);
   ASSERT_NE(components[0], components[1]);
}


TEST(UndirectedGraphTest, Adjacency)
{
   UndirectedGraph ug(3);
   constexpr GraphVertex vertex2 = 2;
   ug.addEdge(1, vertex2);

   std::vector<GraphVertex> neighbors;
   ug.setAdjacentVertices(1, neighbors);
   ASSERT_EQ(Single(neighbors), 2);

   ug.setAdjacentVertices(0, neighbors);
   ASSERT_TRUE(neighbors.empty());

   ug.setAdjacentVertices(2, neighbors);
   ASSERT_EQ(Single(neighbors), 1);
}

TEST(UndirectedGraphTest, AdjacencyPath)
{
   const auto graph = UndirectedGraphLibrary::Get_Path(3);
   std::vector<GraphVertex> neighbors;

   graph->setAdjacentVertices(0, neighbors);
   ASSERT_EQ(Single(neighbors), 1);

   graph->setAdjacentVertices(1, neighbors);
   ASSERT_EQ(neighbors.size(), 2);

   graph->setAdjacentVertices(2, neighbors);
   ASSERT_EQ(Single(neighbors), 1);
}
