#include "UndirectedGraphLibrary.h"
#include "Defines.h"
#include "MyAssert.h"
#include "UndirectedGraphFromG6.h"

#include <numeric>

using namespace Utilities;
using namespace Graph;

namespace
{
std::string CreateName(std::string name, std::initializer_list<Vertex> sizes)
{
    std::string result;
    result += "Homegrown version of " + name;
    for (auto siz : sizes)
    {
        name += " " + std::to_string(siz);
    }
    return name;
}

void SetEdgesOfPath(UndirectedGraph &graph, Edge numEdges)
{
    for (Edge edge : Iota::GetRange(numEdges))
    {
        graph.addEdge(edge, edge + 1);
    }
}

} // namespace

std::unique_ptr<GraphUsc> UndirectedGraphLibrary::Get_Path(Vertex numVertices)
{
    MyAssert(numVertices > 1);
    auto ug = std::make_unique<UndirectedGraph>(numVertices, CreateName("Path", {numVertices}));
    SetEdgesOfPath(*ug, numVertices - 1);
    return std::make_unique<GraphUsc>(*ug);
}

std::unique_ptr<GraphUsc> UndirectedGraphLibrary::Get_Cycle(Vertex numVertices)
{
    MyAssert(numVertices > 2);
    auto ug = std::make_unique<UndirectedGraph>(numVertices, CreateName("Cycle", {numVertices}));
    // add edge connecting first and last
    ug->addEdge(0, numVertices - 1);
    SetEdgesOfPath(*ug, numVertices - 1);
    return std::make_unique<GraphUsc>(*ug);
}

std::unique_ptr<GraphUsc> UndirectedGraphLibrary::Get_CompleteGraph(Vertex numVertices)
{
    MyAssert(numVertices >= 0); // Used to generate empty graph
    auto ug = std::make_unique<UndirectedGraph>(numVertices, CreateName("Complete", {numVertices}));

    for (Vertex n0 : Iota::GetRange(numVertices))
    {
        for (Vertex n1 = n0 + 1; n1 < numVertices; ++n1)
        {
            ug->addEdge(n0, n1);
        }
    }

    return std::make_unique<GraphUsc>(*ug);
}

std::unique_ptr<GraphUsc> UndirectedGraphLibrary::Get_Star(std::initializer_list<Vertex> sizes)
{
    MyAssert(sizes.size() > 2);
    MyAssert(str::all_of(sizes, [](auto v) { return v > 0; }));
    const auto numVertices = str::fold_left(sizes, static_cast<Vertex>(1), std::plus<Vertex>());
    auto ug = std::make_unique<UndirectedGraph>(numVertices, CreateName("Cycle", sizes));
    Vertex cur = 1;
    for (auto s : sizes)
    {
        ug->addEdge(0, cur);
        for (Vertex v : Iota::GetRange(s - 1))
        {
            ug->addEdge(cur + v, cur + v + 1);
        }
        cur += s;
    }
    MyAssert(cur == numVertices);
    return std::make_unique<GraphUsc>(*ug);
}

/*
        2
      /    \
     0 ----  3
       \    /
         1
*/
std::unique_ptr<GraphUsc> UndirectedGraphLibrary::Get_Diamond()
{
    return UndirectedGraphFromG6::CreateConnected(UndirectedGraphFromG6::diamond);
}

/*
      0 -- 2 -- 1
           |
           3
*/
std::unique_ptr<GraphUsc> UndirectedGraphLibrary::Get_Claw()
{
    return UndirectedGraphFromG6::CreateConnected("Cs");
}

/*
       1  -- 3
     / |
    0  |
     \ |
        2 -- 4
*/
std::unique_ptr<GraphUsc> UndirectedGraphLibrary::Get_Bull()
{
    return UndirectedGraphFromG6::CreateConnected("D{O");
}

std::unique_ptr<GraphUsc> UndirectedGraphLibrary::Get_Butterfly()
{
    return UndirectedGraphFromG6::CreateConnected(UndirectedGraphFromG6::butterfly);
}

std::unique_ptr<GraphUsc> UndirectedGraphLibrary::Get_Paw()
{
    return UndirectedGraphFromG6::CreateConnected("Cx");
}

std::unique_ptr<UndirectedGraph> UndirectedGraphLibrary::Get_DisconnectedGraph(Vertex numVertices)
{
    return std::make_unique<UndirectedGraph>(numVertices, CreateName("Disconnected", {numVertices}));
}

std::unique_ptr<GraphUsc> UndirectedGraphLibrary::Get_CompleteBipartite(Vertex size0, Vertex size1)
{
    if (std::min(size0, size1) < 1)
    {
        throw MyException("CompleteBipartiteGraph: both sets muust have positive size");
    }
    UndirectedGraph ug(size0 + size1, CreateName("Complete Bipartite", {size0, size1}));
    for (Vertex n0 : Iota::GetRange(size0))
    {
        for (Vertex n1 : Iota::GetRange(size1))
        {
            ug.addEdge(n0, n1 + size0);
        }
    }
    return std::make_unique<GraphUsc>(ug);
}

std::unique_ptr<Graph::IGraphUsc> UndirectedGraphLibrary::Get_Null()
{
    return Get_CompleteGraph(0);
}

std::unique_ptr<Graph::IGraphUsc> UndirectedGraphLibrary::Get_Singleton()
{
    return Get_CompleteGraph(1);
}
