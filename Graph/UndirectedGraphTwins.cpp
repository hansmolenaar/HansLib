#include "UndirectedGraphTwins.h"

using namespace Graph;

namespace
{
std::vector<std::vector<Vertex>> GetTwins(const std::map<std::vector<Vertex>, std::vector<Vertex>> &twins)
{
    std::vector<std::vector<Vertex>> result;
    for (const auto &twin : twins)
    {
        if (twin.second.size() > 1)
        {
            result.push_back(twin.second);
        }
    }

    return result;
}
} // namespace

UndirectedGraphTwins::UndirectedGraphTwins(const Graph::IGraphUs &graph) : m_graph(graph)
{
    std::map<std::vector<Vertex>, std::vector<Vertex>> trueTwins;
    std::map<std::vector<Vertex>, std::vector<Vertex>> falseTwins;
    std::vector<Vertex> ngbs;

    const auto nVertices = m_graph.getNumVertices();
    for (Vertex v = 0; v < nVertices; ++v)
    {
        m_graph.setAdjacentVertices(v, ngbs);
        str::sort(ngbs);
        falseTwins[ngbs].push_back(v);

        ngbs.push_back(v);
        str::sort(ngbs);
        trueTwins[ngbs].push_back(v);
    }

    m_trueTwins = GetTwins(trueTwins);
    m_falseTwins = GetTwins(falseTwins);
}

const Graph::IGraphUs &UndirectedGraphTwins::getGraph() const
{
    return m_graph;
}

const std::vector<std::vector<Vertex>> &UndirectedGraphTwins::getTrueTwins() const
{
    return m_trueTwins;
}

const std::vector<std::vector<Vertex>> &UndirectedGraphTwins::getFalseTwins() const
{
    return m_falseTwins;
}
