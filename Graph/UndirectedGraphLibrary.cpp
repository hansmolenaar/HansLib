#include "UndirectedGraphLibrary.h"
#include "MyAssert.h"
#include "UndirectedGraphFromG6.h"

using namespace Utilities;

namespace
{
   static void SetEdgesOfPath(UndirectedGraph& graph, GraphEdge numEdges)
   {
      for (GraphEdge edge = 0; edge < numEdges; ++edge)
      {
         graph.addEdge(edge, edge + 1);
      }
   }

   static std::unique_ptr<UndirectedGraph> Create(std::initializer_list<std::pair<GraphVertex, GraphVertex>> edges)
   {
      std::set<GraphVertex> vertices;
      for (auto edge : edges)
      {
         vertices.insert(edge.first);
         vertices.insert(edge.second);
      }

      if (vertices.empty()) return std::make_unique<UndirectedGraph>(0);

      const auto numVertices = *vertices.rbegin() + 1;
      auto result = std::make_unique<UndirectedGraph>(numVertices);

      for (auto edge : edges)
      {
         result->addEdge(edge.first, edge.second);
      }
      return result;
   }

}

std::unique_ptr<UndirectedGraph> UndirectedGraphLibrary::Get_Path(GraphVertex numVertices)
{
   MyAssert(numVertices > 1);
   auto result = std::make_unique<UndirectedGraph>(numVertices);
   SetEdgesOfPath(*result, numVertices - 1);
   return result;
}

std::unique_ptr<UndirectedGraph> UndirectedGraphLibrary::Get_Cycle(GraphVertex numVertices)
{
   MyAssert(numVertices > 2);
   auto result = std::make_unique<UndirectedGraph>(numVertices);
   // add edge connecting first and last
   result->addEdge(0, numVertices - 1);
   SetEdgesOfPath(*result, numVertices - 1);
   return result;
}

std::unique_ptr<UndirectedGraph> UndirectedGraphLibrary::Get_CompleteGraph(GraphVertex numVertices)
{
   MyAssert(numVertices > 0);
   auto result = std::make_unique<UndirectedGraph>(numVertices);

   for (GraphVertex n0 = 0; n0 < numVertices; ++n0)
   {
      for (GraphVertex n1 = n0 + 1; n1 < numVertices; ++n1)
      {
         result->addEdge(n0, n1);
      }
   }

   return result;
}


//      2
//    /    \
//   0 ----  3
//     \    /
//       1
std::unique_ptr<UndirectedGraph> UndirectedGraphLibrary::Get_Diamond()
{
   return UndirectedGraphFromG6::Create("Cz");
}


//    0 -- 2 -- 1
//         |
//         3
std::unique_ptr<UndirectedGraph> UndirectedGraphLibrary::Get_Claw()
{
   return UndirectedGraphFromG6::Create("Cs");
}

//     1  -- 3
//   / |
//  0  |
//   \ |
//      2 -- 4
std::unique_ptr<UndirectedGraph> UndirectedGraphLibrary::Get_Bull()
{
   return UndirectedGraphFromG6::Create("D{O");
}