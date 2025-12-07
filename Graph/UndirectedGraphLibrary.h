#pragma once

#include "GraphUsc.h"
#include "UndirectedGraph.h"

namespace UndirectedGraphLibrary
{
std::unique_ptr<Graph::GraphUsc> Get_Path(Graph::GraphVertex numVertices);
std::unique_ptr<Graph::GraphUsc> Get_Cycle(Graph::GraphVertex numVertices);
std::unique_ptr<Graph::GraphUsc> Get_CompleteGraph(Graph::GraphVertex numVertices);
std::unique_ptr<Graph::UndirectedGraph> Get_DisconnectedGraph(Graph::GraphVertex numVertices);
std::unique_ptr<Graph::GraphUsc> Get_CompleteBipartite(Graph::GraphVertex size0, Graph::GraphVertex size1);
std::unique_ptr<Graph::GraphUsc> Get_Star(std::initializer_list<Graph::GraphVertex> sizes);

std::unique_ptr<Graph::GraphUsc> Get_Diamond();
std::unique_ptr<Graph::GraphUsc> Get_Claw();
std::unique_ptr<Graph::GraphUsc> Get_Bull();
std::unique_ptr<Graph::GraphUsc> Get_Butterfly();
std::unique_ptr<Graph::GraphUsc> Get_Paw();

std::unique_ptr<Graph::IGraphUsc> Get_Null();
std::unique_ptr<Graph::IGraphUsc> Get_Singleton();
} // namespace UndirectedGraphLibrary
