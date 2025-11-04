#pragma once

#include "GraphUsc.h"
#include "UndirectedGraph.h"

namespace UndirectedGraphLibrary
{
std::unique_ptr<Graph::GraphUsc> Get_Path(GraphVertex numVertices);
std::unique_ptr<Graph::GraphUsc> Get_Cycle(GraphVertex numVertices);
std::unique_ptr<Graph::GraphUsc> Get_CompleteGraph(GraphVertex numVertices);
std::unique_ptr<Graph::UndirectedGraph> Get_DisconnectedGraph(GraphVertex numVertices);
std::unique_ptr<Graph::GraphUsc> Get_CompleteBipartite(GraphVertex size0, GraphVertex size1);
std::unique_ptr<Graph::GraphUsc> Get_Star(std::initializer_list<GraphVertex> sizes);

std::unique_ptr<Graph::GraphUsc> Get_Diamond();
std::unique_ptr<Graph::GraphUsc> Get_Claw();
std::unique_ptr<Graph::GraphUsc> Get_Bull();
std::unique_ptr<Graph::GraphUsc> Get_Butterfly();
std::unique_ptr<Graph::GraphUsc> Get_Paw();

std::unique_ptr<Graph::IGraphUSC> Get_Null();
std::unique_ptr<Graph::IGraphUSC> Get_Singleton();
} // namespace UndirectedGraphLibrary
