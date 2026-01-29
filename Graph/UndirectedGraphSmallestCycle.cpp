#include "UndirectedGraphSmallestCycle.h"
#include "FirstDuplicate.h"
#include "IGraphUs.h"
#include "MyAssert.h"

using namespace Graph;
using namespace Utilities;

UndirectedGraphSmallestCycle::UndirectedGraphSmallestCycle(const Graph::IGraphUs &graph)
    : m_graph(graph), m_lengths(m_graph.getNumVertices())
{
    str::transform(m_graph.getVertexRange(), m_lengths.begin(),
                   [&graph](Vertex v) { return getSmallestCycleLength(graph, v); });
}

const IGraphUs &UndirectedGraphSmallestCycle::getGraph() const
{
    return m_graph;
}

const std::vector<Graph::Vertex> &UndirectedGraphSmallestCycle::getSmallestCycleLengths() const
{
    return m_lengths;
}

namespace
{
Vertex CheckLoop(const std::map<Vertex, Vertex> &vertex2from, Vertex start, Vertex end0, Vertex end1)
{
    std::vector<Vertex> loop;
    Vertex v = end0;
    while (vertex2from.at(v) != start)
    {
        v = vertex2from.at(v);
        loop.push_back(v);
    }

    v = end1;
    while (vertex2from.at(v) != start)
    {
        v = vertex2from.at(v);
        loop.push_back(v);
    }
    // TODO no need to copy
    if (firstDuplicate(loop).has_value())
    {
        return UndirectedGraphSmallestCycle::noCycleLength;
    }
    return loop.size() + 3;
}
} // namespace

Vertex UndirectedGraphSmallestCycle::getSmallestCycleLength(const IGraphUs &graph, Vertex start)
{
    std::map<Vertex, Vertex> vertex2from;

    std::vector<Vertex> cur{start};
    std::vector<Vertex> nxt;
    std::vector<Vertex> ngbs;
    std::set<Vertex> done;

    while (!cur.empty())
    {
        for (Vertex c : cur)
        {
            graph.setAdjacentVertices(c, ngbs);
            for (Vertex n : ngbs)
            {
                if (vertex2from.contains(n))
                {
                    if (!done.contains(n))
                    {
                        const auto len = CheckLoop(vertex2from, start, c, n);
                        if (len != noCycleLength)
                        {
                            return len;
                        }
                    }
                    else
                    {
                        // Back edge, continue
                    }
                }
                else
                {
                    vertex2from[n] = c;
                    nxt.push_back(n);
                }
            }
        }

        // TODO
        for (Vertex c : cur)
        {
            done.insert(c);
        }
        cur.clear();
        std::swap(cur, nxt);
    }

    return noCycleLength;
}
