#include "Defines.h"
#include "MyAssert.h"
#include "UndirectedGraph.h"
#include "UndirectedGraphDistance.h"

using namespace Utilities;

UndirectedGraphDistance::UndirectedGraphDistance(const UndirectedGraph &graph) : m_graph(graph)
{
}

UndirectedGraphDistance::AtDistance UndirectedGraphDistance::operator()(GraphVertex vertex) const
{
    const auto nVertices = m_graph.getNumVertices();
    std::set<GraphVertex> done;

    AtDistance result;

    std::vector<GraphVertex> ngbs;
    std::vector<GraphVertex> current(1, vertex);

    while (!current.empty())
    {
        result.push_back(current);
        for (auto c : current)
        {
            done.insert(c);
        }
        std::set<GraphVertex> todo;
        for (auto c : current)
        {
            m_graph.setAdjacentVertices(c, ngbs);
            for (auto n : ngbs)
            {
                if (!done.contains(n))
                {
                    todo.insert(n);
                }
            }
        }
        current.clear();
        // TODO use c++23
        // current.append_range(current.begin(), todo);
        for (auto v : todo)
        {
            current.push_back(v);
        }
    }

    return result;
}
