#pragma once

#include "UndirectedGraph.h"

namespace UndirectedGraphLibrary
{
   std::unique_ptr<UndirectedGraph> Get_Path(GraphVertex numVertices);
   std::unique_ptr<UndirectedGraph> Get_Cycle(GraphVertex numVertices);
   std::unique_ptr<UndirectedGraph> Get_CompleteGraph(GraphVertex numVertices);

   std::unique_ptr<UndirectedGraph> Get_Diamond();
   std::unique_ptr<UndirectedGraph> Get_Claw();
   std::unique_ptr<UndirectedGraph> Get_Bull();
}