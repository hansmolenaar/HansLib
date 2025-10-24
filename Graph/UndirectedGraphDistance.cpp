#include "UndirectedGraphDistance.h"
#include "Defines.h"
#include "MyAssert.h"
#include "UndirectedGraph.h"

using namespace Utilities;

namespace
{
UndirectedGraphDistance::AtDistance Generate(const UndirectedGraph &graph, const GraphVertex vertex)
{
    const auto nVertices = graph.getNumVertices();
    std::vector<bool> done(nVertices, false);

    UndirectedGraphDistance::AtDistance result;

    std::vector<GraphVertex> ngbs;
    std::vector<GraphVertex> current(1, vertex);

    while (!current.empty())
    {
        result.push_back(current);
        for (auto c : current)
        {
            done[c] = true;
        }
        std::set<GraphVertex> todo;
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
        current = std::vector<GraphVertex>(todo.begin(), todo.end());
        // TODO use c++23
        // current = std::vector<GraphVertex>(todo);
    }

    return result;
}
} // namespace

UndirectedGraphDistance::UndirectedGraphDistance(const UndirectedGraph &graph) : m_graph(graph)
{
    const auto nVertices = graph.getNumVertices();
    m_distances.reserve(nVertices);
    for (GraphVertex v = 0; v < nVertices; ++v)
    {
        m_distances.emplace_back(Generate(m_graph, v));
    }
}

const UndirectedGraphDistance::AtDistance &UndirectedGraphDistance::operator()(GraphVertex vertex) const
{
    return m_distances.at(vertex);
}
