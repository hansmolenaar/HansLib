#pragma once

#include "GraphUsc.h"
#include "UndirectedGraph.h"

namespace UndirectedGraphLibrary
{
std::unique_ptr<Graph::GraphUsc> Get_Path(Graph::Vertex numVertices);
std::unique_ptr<Graph::GraphUsc> Get_Cycle(Graph::Vertex numVertices);
std::unique_ptr<Graph::GraphUsc> Get_CompleteGraph(Graph::Vertex numVertices);
std::unique_ptr<Graph::UndirectedGraph> Get_DisconnectedGraph(Graph::Vertex numVertices);
std::unique_ptr<Graph::GraphUsc> Get_CompleteBipartite(Graph::Vertex size0, Graph::Vertex size1);
std::unique_ptr<Graph::GraphUsc> Get_Star(std::initializer_list<Graph::Vertex> sizes);

std::unique_ptr<Graph::GraphUsc> Get_Diamond();
std::unique_ptr<Graph::GraphUsc> Get_Claw();
std::unique_ptr<Graph::GraphUsc> Get_Bull();
std::unique_ptr<Graph::GraphUsc> Get_Butterfly();
std::unique_ptr<Graph::GraphUsc> Get_Paw();

std::unique_ptr<Graph::IGraphUsc> Get_Null();
std::unique_ptr<Graph::IGraphUsc> Get_Singleton();

Graph::UndirectedGraph Get_Prism(Graph::Vertex numVertices);
Graph::UndirectedGraph Get_DistortedPrism(Graph::Vertex numVertices, Graph::Vertex (*connectTo)(Graph::Vertex));

std::vector<std::unique_ptr<Graph::IGraphUs>> Get_GraphsOrderLE5();

} // namespace UndirectedGraphLibrary
