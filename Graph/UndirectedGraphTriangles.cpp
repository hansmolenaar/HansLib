#include "UndirectedGraphTriangles.h"

using namespace Graph;

namespace
{

std::vector<std::array<Vertex, 3>> GetAllTriangles(const Graph::IGraphUs &graph)
{
    const auto nVertices = graph.getNumVertices();
    std::vector<std::vector<Vertex>> allNeighbors(nVertices);
    for (Vertex v = 0; v < nVertices; ++v)
    {
        graph.setAdjacentVertices(v, allNeighbors.at(v));
    }

    std::vector<std::array<Vertex, 3>> result;
    std::vector<Vertex> commonVertices;
    for (const auto &edge : graph.getAllSortedEdges())
    {
        commonVertices.clear();
        str::set_intersection(allNeighbors.at(edge[0]), allNeighbors.at(edge[1]), std::back_inserter(commonVertices));
        for (Vertex commonVertex : commonVertices)
        {
            if (commonVertex > edge[1])
            {
                result.emplace_back(std::array<Vertex, 3>{edge[0], edge[1], commonVertex});
            }
        }
    }

    return result;
}

} // namespace

UndirectedGraphTriangles::UndirectedGraphTriangles(const Graph::IGraphUs &graph)
    : m_graph(graph), m_countPerVertex(m_graph.getNumVertices()), m_numNeigborsInTriangles(m_graph.getNumVertices())
{
    const auto allTriangles = GetAllTriangles(graph);
    std::map<Vertex, std::set<Vertex>> neighbors;
    for (const auto &triangle : allTriangles)
    {
        for (Vertex vertex : triangle)
        {
            m_countPerVertex.at(vertex) += 1;
            for (auto ngb : triangle)
            {
                neighbors[vertex].insert(ngb);
            }
        }
    }

    for (const auto &itr : neighbors)
    {
        m_numNeigborsInTriangles[itr.first] = itr.second.size() - 1;
    }
}

std::vector<size_t> UndirectedGraphTriangles::getSequence() const
{
    return m_countPerVertex;
}

size_t UndirectedGraphTriangles::numTrianglesAt(Vertex vertex) const
{
    return m_countPerVertex.at(vertex);
}

const Graph::IGraphUs &UndirectedGraphTriangles::getGraph() const
{
    return m_graph;
}

size_t UndirectedGraphTriangles::numNeighborsInTrianglesAt(Vertex vertex) const
{
    return m_numNeigborsInTriangles.at(vertex);
}
