#include <gtest/gtest.h>

#include "UndirectedGraphLibrary.h"
#include "Defines.h"
#include "Single.h"


using namespace Utilities;

namespace
{
   void Expect(const UndirectedGraph& graph, GraphVertex numVertices, std::span<const GraphVertex> degreeSequence)
   {
      ASSERT_EQ(graph.getNumVertices(), numVertices);
      ASSERT_EQ(degreeSequence.size(), numVertices);
      const auto degrees = graph.getDegreeSequence();
      ASSERT_TRUE(std::equal(degrees.begin(), degrees.end(), degreeSequence.begin(), degreeSequence.end()));
   }
}
TEST(UndirectedGraphTest, ConnectedComponents)
{
   UndirectedGraph ug(3);

   ASSERT_EQ(ug.getNumVertices(), 3);
   ASSERT_EQ(ug.getNumEdges(), 0);

   constexpr GraphVertex vertex2 = 2;
   ug.addEdge(1, vertex2);
   ASSERT_EQ(ug.getNumEdges(), 1);
   ASSERT_EQ(ug.getDegree(0), 0);
   ASSERT_EQ(ug.getDegree(1), 1);
   ASSERT_EQ(ug.getDegree(2), 1);

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


TEST(UndirectedGraphTest, GetCycle)
{
   constexpr size_t maxNumVertices = 10;
   for (size_t numVertices = 3; numVertices < maxNumVertices; ++numVertices)
   {
      const auto graph = UndirectedGraphLibrary::Get_Cycle(numVertices);
      ASSERT_EQ(graph->getNumVertices(), numVertices);
      ASSERT_EQ(graph->getNumEdges(), numVertices);
      for (GraphVertex v = 0; v < numVertices; ++v)
      {
         ASSERT_EQ(graph->getDegree(v), 2);
      }
   }
}


TEST(UndirectedGraphTest, GetCompleteGraph)
{
   constexpr size_t maxNumVertices = 6;
   for (size_t numVertices = 1; numVertices < maxNumVertices; ++numVertices)
   {
      const auto graph = UndirectedGraphLibrary::Get_CompleteGraph(numVertices);
      ASSERT_EQ(graph->getNumVertices(), numVertices);
      ASSERT_EQ(graph->getNumEdges(), numVertices * (numVertices - 1) / 2);
      for (GraphVertex v = 0; v < numVertices; ++v)
      {
         ASSERT_EQ(graph->getDegree(v), numVertices - 1);
      }
   }
}

TEST(UndirectedGraphTest, GetDiamond)
{
   const auto graph = UndirectedGraphLibrary::Get_Diamond();
   Expect(*graph, 4, std::vector<GraphVertex>{2, 2, 3, 3});
}

TEST(UndirectedGraphTest, GetClaw)
{
   const auto graph = UndirectedGraphLibrary::Get_Claw();
   Expect(*graph, 4, std::vector<GraphVertex>{1, 1, 1, 3});
}

TEST(UndirectedGraphTest, GetBull)
{
   const auto graph = UndirectedGraphLibrary::Get_Bull();
   Expect(*graph, 5, std::vector<GraphVertex>{1, 1, 2, 3, 3});
}
