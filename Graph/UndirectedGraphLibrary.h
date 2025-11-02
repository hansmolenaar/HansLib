#pragma once

#include "UndirectedGraph.h"
#include "UscGraph.h"

namespace UndirectedGraphLibrary
{
std::unique_ptr<Graph::UscGraph> Get_Path(GraphVertex numVertices);
std::unique_ptr<Graph::UscGraph> Get_Cycle(GraphVertex numVertices);
std::unique_ptr<Graph::UscGraph> Get_CompleteGraph(GraphVertex numVertices);
std::unique_ptr<UndirectedGraph> Get_DisconnectedGraph(GraphVertex numVertices);
std::unique_ptr<Graph::UscGraph> Get_CompleteBipartite(GraphVertex size0, GraphVertex size1);
std::unique_ptr<Graph::UscGraph> Get_Star(std::initializer_list<GraphVertex> sizes);

std::unique_ptr<Graph::UscGraph> Get_Diamond();
std::unique_ptr<Graph::UscGraph> Get_Claw();
std::unique_ptr<Graph::UscGraph> Get_Bull();
std::unique_ptr<Graph::UscGraph> Get_Butterfly();
std::unique_ptr<Graph::UscGraph> Get_Paw();
} // namespace UndirectedGraphLibrary
