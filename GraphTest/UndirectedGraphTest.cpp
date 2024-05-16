#include <gtest/gtest.h>

#include "UndirectedGraphLibrary.h"
#include "Defines.h"
#include "Single.h"


using namespace Utilities;

namespace
{
   void CheckSequenceAfterSorting(std::span<const GraphVertex> actual, std::span<const GraphVertex> expectSorted)
   {
      std::vector<GraphVertex> actualSorted(actual.begin(), actual.end());
      str::sort(actualSorted);
      if (!std::equal(actualSorted.begin(), actualSorted.end(), expectSorted.begin(), expectSorted.end()))
      {
         int hello = 1;
      }
      ASSERT_TRUE(std::equal(actualSorted.begin(), actualSorted.end(), expectSorted.begin(), expectSorted.end()));
   }

   void CheckDegreeSequence(const UndirectedGraph& graph, GraphVertex numVertices, std::span<const GraphVertex> degreeSequenceSorted)
   {
      ASSERT_EQ(graph.getNumVertices(), numVertices);
      ASSERT_EQ(degreeSequenceSorted.size(), numVertices);
      const auto degrees = graph.getDegreeSequence();
      CheckSequenceAfterSorting(degrees, degreeSequenceSorted);
   }

   std::vector<GraphVertex> GetCyclePathSize(const std::vector<std::vector<GraphVertex>>& cyclesPaths)
   {
      std::vector<GraphVertex>  result;
      for (const auto& cp : cyclesPaths)
      {
         result.push_back(cp.size());
      }
      return result;
   }

   void CheckCyclesPaths(const UndirectedGraph& graph, std::span<const GraphVertex> cycleSizesExpect, std::span<const GraphVertex> pathSizesExpect)
   {
      std::vector<std::vector<GraphVertex>> cycles;
      std::vector<std::vector<GraphVertex>> paths;

      const UndirectedGraph::CyclesAndPaths  cyclesAndPaths = graph.SplitInCyclesAndPaths();
      CheckSequenceAfterSorting(GetCyclePathSize(cyclesAndPaths.Cycles), cycleSizesExpect);
      CheckSequenceAfterSorting(GetCyclePathSize(cyclesAndPaths.Paths), pathSizesExpect);
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
   CheckDegreeSequence(*graph, 4, std::vector<GraphVertex>{2, 2, 3, 3});
}

TEST(UndirectedGraphTest, GetClaw)
{
   const auto graph = UndirectedGraphLibrary::Get_Claw();
   CheckDegreeSequence(*graph, 4, std::vector<GraphVertex>{1, 1, 1, 3});
}

TEST(UndirectedGraphTest, GetBull)
{
   const auto graph = UndirectedGraphLibrary::Get_Bull();
   CheckDegreeSequence(*graph, 5, std::vector<GraphVertex>{1, 1, 2, 3, 3});
}


TEST(UndirectedGraphTest, SplitInCyclesAndPaths)
{
   std::vector<std::vector<GraphVertex>> cycles;
   std::vector<std::vector<GraphVertex>> paths;

   auto graph = UndirectedGraphLibrary::Get_Path(2);
   CheckCyclesPaths(*graph, std::vector<GraphVertex>{}, std::vector<GraphVertex>{2});

   graph = UndirectedGraphLibrary::Get_Path(3);
   CheckCyclesPaths(*graph, std::vector<GraphVertex>{}, std::vector<GraphVertex>{3});

   graph = UndirectedGraphLibrary::Get_Cycle(3);
   CheckCyclesPaths(*graph, std::vector<GraphVertex>{3}, std::vector<GraphVertex>{});

   graph = UndirectedGraphLibrary::Get_Cycle(4);
   CheckCyclesPaths(*graph, std::vector<GraphVertex>{4}, std::vector<GraphVertex>{});

   graph = UndirectedGraphLibrary::Get_Claw();
   CheckCyclesPaths(*graph, std::vector<GraphVertex>{}, std::vector<GraphVertex>{2, 2, 2});

   graph = UndirectedGraphLibrary::Get_Diamond();
   CheckCyclesPaths(*graph, std::vector<GraphVertex>{}, std::vector<GraphVertex>{2, 3, 3});

   graph = UndirectedGraphLibrary::Get_Bull();
   CheckCyclesPaths(*graph, std::vector<GraphVertex>{}, std::vector<GraphVertex>{2, 2, 2, 3});

   graph = UndirectedGraphLibrary::Get_Paw();
   CheckCyclesPaths(*graph, std::vector<GraphVertex>{3}, std::vector<GraphVertex>{2});

   graph = UndirectedGraphLibrary::Get_Butterfly();
   CheckCyclesPaths(*graph, std::vector<GraphVertex>{3, 3}, std::vector<GraphVertex>{});

   for (GraphVertex numVertices = 0; numVertices < 4; ++numVertices)
   {
      graph = UndirectedGraphLibrary::Get_DisconnectedGraph(numVertices);
      CheckCyclesPaths(*graph, std::vector<GraphVertex>{}, std::vector<GraphVertex>{});
   }

   graph = UndirectedGraphLibrary::Get_CompleteBipartite(3, 3);
   CheckCyclesPaths(*graph, std::vector<GraphVertex>{}, std::vector<GraphVertex>(9, 2));
}


TEST(UndirectedGraphTest, AddEdgeDuplicate)
{
   UndirectedGraph ug(2);
   ug.addEdge(0, 1);
   ug.addEdge(1, 0);
   ug.addEdge(0, 1);
   ug.addEdge(1, 0);

   ASSERT_EQ(ug.getNumEdges(), 1);
}


TEST(UndirectedGraphTest, AreAdjacent)
{
   UndirectedGraph ug(3);
   ug.addEdge(0, 1);
   ASSERT_TRUE(ug.areAdjacent(0, 1));
   ASSERT_TRUE(ug.areAdjacent(1, 0));
   ASSERT_FALSE(ug.areAdjacent(1, 2));
   ASSERT_ANY_THROW(ug.areAdjacent(1, 3));
}
