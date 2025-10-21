#pragma once

#include "UndirectedGraph.h"

namespace UndirectedGraphLibrary
{
std::unique_ptr<UndirectedGraph> Get_Path(GraphVertex numVertices);
std::unique_ptr<UndirectedGraph> Get_Cycle(GraphVertex numVertices);
std::unique_ptr<UndirectedGraph> Get_CompleteGraph(GraphVertex numVertices);
std::unique_ptr<UndirectedGraph> Get_DisconnectedGraph(GraphVertex numVertices);
std::unique_ptr<UndirectedGraph> Get_CompleteBipartite(GraphVertex size0, GraphVertex size1);

std::unique_ptr<UndirectedGraph> Get_Diamond();
std::unique_ptr<UndirectedGraph> Get_Claw();
std::unique_ptr<UndirectedGraph> Get_Bull();
std::unique_ptr<UndirectedGraph> Get_Butterfly();
std::unique_ptr<UndirectedGraph> Get_Paw();
} // namespace UndirectedGraphLibrary