#include "UndirectedGraphLibrary.h"
#include "Defines.h"
#include "MyAssert.h"
#include "UndirectedGraphFromG6.h"

#include <numeric>

using namespace Utilities;
using namespace Graph;

namespace
{
std::string CreateName(std::string name, std::initializer_list<GraphVertex> sizes)
{
    std::string result;
    result += "Homegrown version of " + name;
    for (auto siz : sizes)
    {
        name += " " + std::to_string(siz);
    }
    return name;
}

void SetEdgesOfPath(UndirectedGraph &graph, GraphEdge numEdges)
{
    for (GraphEdge edge = 0; edge < numEdges; ++edge)
    {
        graph.addEdge(edge, edge + 1);
    }
}

std::unique_ptr<UndirectedGraph> Create(std::initializer_list<std::pair<GraphVertex, GraphVertex>> edges)
{
    std::set<GraphVertex> vertices;
    for (auto edge : edges)
    {
        vertices.insert(edge.first);
        vertices.insert(edge.second);
    }

    if (vertices.empty())
        return std::make_unique<UndirectedGraph>(0);

    const auto numVertices = *vertices.rbegin() + 1;
    auto result = std::make_unique<UndirectedGraph>(numVertices);

    for (auto edge : edges)
    {
        result->addEdge(edge.first, edge.second);
    }
    return result;
}

} // namespace

std::unique_ptr<GraphUsc> UndirectedGraphLibrary::Get_Path(GraphVertex numVertices)
{
    MyAssert(numVertices > 1);
    auto ug = std::make_unique<UndirectedGraph>(numVertices, CreateName("Path", {numVertices}));
    SetEdgesOfPath(*ug, numVertices - 1);
    return std::make_unique<GraphUsc>(*ug);
}

std::unique_ptr<GraphUsc> UndirectedGraphLibrary::Get_Cycle(GraphVertex numVertices)
{
    MyAssert(numVertices > 2);
    auto ug = std::make_unique<UndirectedGraph>(numVertices, CreateName("Cycle", {numVertices}));
    // add edge connecting first and last
    ug->addEdge(0, numVertices - 1);
    SetEdgesOfPath(*ug, numVertices - 1);
    return std::make_unique<GraphUsc>(*ug);
}

std::unique_ptr<GraphUsc> UndirectedGraphLibrary::Get_CompleteGraph(GraphVertex numVertices)
{
    MyAssert(numVertices >= 0); // Used to generate empty graph
    auto ug = std::make_unique<UndirectedGraph>(numVertices, CreateName("Complete", {numVertices}));

    for (GraphVertex n0 = 0; n0 < numVertices; ++n0)
    {
        for (GraphVertex n1 = n0 + 1; n1 < numVertices; ++n1)
        {
            ug->addEdge(n0, n1);
        }
    }

    return std::make_unique<GraphUsc>(*ug);
}

std::unique_ptr<GraphUsc> UndirectedGraphLibrary::Get_Star(std::initializer_list<GraphVertex> sizes)
{
    MyAssert(sizes.size() > 2);
    MyAssert(str::all_of(sizes, [](auto v) { return v > 0; }));
    const auto numVertices = str::fold_left(sizes, static_cast<GraphVertex>(1), std::plus<GraphVertex>());
    auto ug = std::make_unique<UndirectedGraph>(numVertices, CreateName("Cycle", sizes));
    GraphVertex cur = 1;
    for (auto s : sizes)
    {
        ug->addEdge(0, cur);
        for (GraphVertex v = 0; v < s - 1; ++v)
        {
            ug->addEdge(cur + v, cur + v + 1);
        }
        cur += s;
    }
    MyAssert(cur == numVertices);
    return std::make_unique<GraphUsc>(*ug);
}

//      2
//    /    \
//   0 ----  3
//     \    /
//       1
std::unique_ptr<GraphUsc> UndirectedGraphLibrary::Get_Diamond()
{
    return UndirectedGraphFromG6::CreateConnected(UndirectedGraphFromG6::diamond);
}

//    0 -- 2 -- 1
//         |
//         3
std::unique_ptr<GraphUsc> UndirectedGraphLibrary::Get_Claw()
{
    return UndirectedGraphFromG6::CreateConnected("Cs");
}

//     1  -- 3
//   / |
//  0  |
//   \ |
//      2 -- 4
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

std::unique_ptr<UndirectedGraph> UndirectedGraphLibrary::Get_DisconnectedGraph(GraphVertex numVertices)
{
    return std::make_unique<UndirectedGraph>(numVertices, CreateName("Disconnected", {numVertices}));
}

std::unique_ptr<GraphUsc> UndirectedGraphLibrary::Get_CompleteBipartite(GraphVertex size0, GraphVertex size1)
{
    if (std::min(size0, size1) < 1)
    {
        throw MyException("CompleteBipartiteGraph: both sets muust have positive size");
    }
    UndirectedGraph ug(size0 + size1, CreateName("Complete Bipartite", {size0, size1}));
    for (int n0 = 0; n0 < size0; ++n0)
    {
        for (int n1 = 0; n1 < size1; ++n1)
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
