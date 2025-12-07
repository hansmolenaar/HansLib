#include "UndirectedGraphDistance.h"
#include "Defines.h"
#include "MyAssert.h"
#include "UndirectedGraph.h"

using namespace Utilities;
using namespace Graph;

namespace
{
UndirectedGraphDistance::AtDistance Generate(const IGraphUs &graph, const Vertex vertex)
{
    const auto nVertices = graph.getNumVertices();
    std::vector<bool> done(nVertices, false);

    UndirectedGraphDistance::AtDistance result;

    std::vector<Vertex> ngbs;
    std::vector<Vertex> current(1, vertex);

    while (!current.empty())
    {
        result.push_back(current);
        for (auto c : current)
        {
            done[c] = true;
        }
        std::set<Vertex> todo;
        for (auto c : current)
        {
            graph.setAdjacentVertices(c, ngbs);
            for (auto n : ngbs)
            {
                if (!done.at(n))
                {
                    todo.insert(n);
                }
            }
        }
        current = std::vector<Vertex>(todo.begin(), todo.end());
        // TODO use c++23
        // current = std::vector<Vertex>(todo);
    }

    return result;
}
} // namespace

UndirectedGraphDistance::UndirectedGraphDistance(const IGraphUs &graph) : m_graph(graph)
{
    const auto nVertices = graph.getNumVertices();
    m_distances.reserve(nVertices);
    for (Vertex v = 0; v < nVertices; ++v)
    {
        m_distances.emplace_back(Generate(graph, v));
    }
}

const Graph::IGraphUs &UndirectedGraphDistance::getGraph() const
{
    return m_graph;
}

const UndirectedGraphDistance::AtDistance &UndirectedGraphDistance::operator()(Vertex vertex) const
{
    return m_distances.at(vertex);
}

Vertex UndirectedGraphDistance::getNumVertices() const
{
    return m_distances.size();
}
