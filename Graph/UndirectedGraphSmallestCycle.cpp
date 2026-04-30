#include "UndirectedGraphSmallestCycle.h"
#include "FirstDuplicate.h"
#include "IGraphUs.h"
#include "MyAssert.h"

using namespace Graph;
using namespace Utilities;

UndirectedGraphSmallestCycle::UndirectedGraphSmallestCycle(const Graph::IGraphUs &graph)
    : m_graph(graph), m_lengths(m_graph.getNumVertices())
{
    str::transform(m_graph.getVertexRange(), m_lengths.begin(), [&graph](Vertex v) {
        const auto retval = getSmallestCycleLength(graph, v);
        return retval ? *retval : noCycleLength;
    });
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
std::optional<Vertex> CheckLoop(const std::map<Vertex, Vertex> &vertex2from, Vertex start, Vertex end0, Vertex end1)
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

    // Check for duplicates
    str::sort(loop);
    if (str::adjacent_find(loop) != loop.end())
    {
        return {};
    }
    return loop.size() + 3;
}
} // namespace

std::optional<Vertex> UndirectedGraphSmallestCycle::getSmallestCycleLength(const IGraphUs &graph, Vertex start)
{
    std::map<Vertex, Vertex> vertex2from;

    std::vector<Vertex> cur{start};
    std::vector<Vertex> nxt;
    std::vector<Vertex> ngbs;
    std::optional<Vertex> optimal;

    while (!cur.empty())
    {
        for (Vertex c : cur)
        {
            graph.setAdjacentVertices(c, ngbs);
            for (Vertex n : ngbs)
            {
                if (vertex2from.contains(c) && vertex2from.find(c)->second == n)
                {
                    // Back edge, continue
                    continue;
                }
                if (vertex2from.contains(n))
                {
                    const auto len = CheckLoop(vertex2from, start, c, n);
                    if (len)
                    {
                        optimal = optimal ? std::min(*optimal, *len) : *len;
                    }
                }
                else
                {
                    vertex2from[n] = c;
                    nxt.push_back(n);
                }
            }
        }

        if (optimal)
        {
            return optimal;
        }

        cur.clear();
        std::swap(cur, nxt);
    }

    return {};
}
