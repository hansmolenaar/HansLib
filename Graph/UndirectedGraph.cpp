#include "UndirectedGraph.h"
#include "Defines.h"
#include "MyAssert.h"

#include <sstream>

using namespace Graph;
using namespace Utilities;

namespace
{
constexpr Vertex DegreeSequenceDone = std::numeric_limits<Vertex>::max();

// If a polygon is found the last node of the resulting list equals the first one
std::vector<Vertex> TraceLineOrPolygon(const UndirectedGraph &graph, Vertex start, Vertex nxt,
                                       std::vector<Vertex> &degreeSequence)
{
    std::vector<Vertex> result;
    std::vector<Vertex> ngbVertices;
    result.push_back(start);
    while (true)
    {
        result.push_back(nxt);
        if (nxt == start)
        {
            // interesting, a loop
            MyAssert(result.size() > 3); // not a proper labeled graph: loop
            return result;
        }
        else if (degreeSequence[nxt] == 2)
        {
            degreeSequence[nxt] = DegreeSequenceDone; // Mark as done
            // Find next vertex
            graph.setAdjacentVertices(nxt, ngbVertices);
            MyAssert(ngbVertices.size() == 2);
            nxt = (ngbVertices[0] == result[result.size() - 2] ? ngbVertices[1] : ngbVertices[0]);
        }
        else
        {
            if (degreeSequence.at(nxt) == 1)
            {
                degreeSequence.at(nxt) = DegreeSequenceDone;
            }
            // Last point of line reached
            return result;
        }
    }
}
} // namespace

UndirectedGraph::UndirectedGraph(Vertex numVertices, std::string name) : m_graph(numVertices), m_name(name)
{
}

UndirectedGraph::UndirectedGraph(const Graph::IGraphUs &graph)
    : m_graph(graph.getNumVertices()), m_name(graph.getName())
{
    const auto numVertices = graph.getNumVertices();
    std::vector<Vertex> ngbs;
    for (Vertex v = 0; v < numVertices; ++v)
    {
        graph.setAdjacentVertices(v, ngbs);
        for (Vertex n : ngbs)
        {
            if (v < n)
            {
                addEdge(v, n);
            }
        }
    }
}

void UndirectedGraph::addEdge(Vertex vertex1, Vertex vertex2)
{
    if (!areAdjacent(vertex1, vertex2))
    {
        boost::add_edge(vertex1, vertex2, m_graph);
    }
}

void UndirectedGraph::addEdge(VertexPair edge)
{
    addEdge(edge[0], edge[1]);
}

void UndirectedGraph::addWalk(std::initializer_list<Vertex> walk)
{
    auto size = walk.size();
    if (size < 2)
    {
        return;
    }
    auto cur = walk.begin();
    auto nxt = cur + 1;
    --size;
    while (size > 0)
    {
        addEdge(*cur, *nxt);
        cur = nxt;
        nxt = cur + 1;
        --size;
    }
}

Vertex UndirectedGraph::getNumVertices() const
{
    return boost::num_vertices(m_graph);
}

std::vector<Vertex> UndirectedGraph::getConnectedComponents() const
{
    std::vector<Vertex> result(getNumVertices());
    boost::connected_components(m_graph, result.data());
    return result;
}

void UndirectedGraph::setAdjacentVertices(Vertex vertex, std::vector<Vertex> &result) const
{
    result.clear();
    const auto neighbours = boost::adjacent_vertices(vertex, m_graph);
    for (auto vd : make_iterator_range(neighbours))
    {
        result.push_back(vd);
    }
    str::sort(result);
}

Edge UndirectedGraph::getNumEdges() const
{
    return boost::num_edges(m_graph);
}

Vertex UndirectedGraph::getDegree(Vertex vertex) const
{
    const auto neighbours = boost::adjacent_vertices(vertex, m_graph);
    return std::distance(neighbours.first, neighbours.second);
}

std::vector<Vertex> UndirectedGraph::getDegreeSequence() const
{
    const auto numVertices = getNumVertices();
    std::vector<Vertex> result(numVertices);
    for (Vertex v = 0; v < numVertices; ++v)
    {
        result[v] = getDegree(v);
    }
    return result;
}

// Isolated vertices are ignored
UndirectedGraph::CyclesAndPaths UndirectedGraph::SplitInCyclesAndPaths() const
{
    CyclesAndPaths result;

    const auto vertexCount = getNumVertices();
    auto degreeSequence = getDegreeSequence();
    std::vector<Vertex> ngbVertices;

    for (Vertex v = 0; v < vertexCount; ++v)
    {
        // Set degree sequence to -1 if done
        if (degreeSequence.at(v) != DegreeSequenceDone && degreeSequence.at(v) != 2)
        {
            // New lines starts here, loop over the edges that contain this node
            // neighbors.
            setAdjacentVertices(v, ngbVertices);
            for (auto ngb : ngbVertices)
            {
                if (degreeSequence.at(ngb) != DegreeSequenceDone)
                {
                    std::vector<Vertex> list = TraceLineOrPolygon(*this, v, ngb, degreeSequence);
                    if (list.front() != list.back())
                    {
                        result.Paths.emplace_back(std::move(list));
                    }
                    else
                    {
                        list.pop_back();
                        result.Cycles.emplace_back(std::move(list));
                    }
                }
            }
            degreeSequence.at(v) = DegreeSequenceDone; // Done here
        }
    }

    // But there is more: isolated loops
    for (Vertex v = 0; v < vertexCount; ++v)
    {
        if (degreeSequence.at(v) == 2)
        {
            setAdjacentVertices(v, ngbVertices);
            MyAssert(ngbVertices.size() == 2);
            auto list = TraceLineOrPolygon(*this, v, ngbVertices.front(), degreeSequence);
            MyAssert(list.front() == list.back());
            list.pop_back(); // remove last
            result.Cycles.emplace_back(std::move(list));
            degreeSequence.at(v) = DegreeSequenceDone; // Done here
        }
    }

    MyAssert(str::all_of(degreeSequence, [](Vertex v) { return v == DegreeSequenceDone; }));
    return result;
}

bool UndirectedGraph::areAdjacent(Vertex v1, Vertex v2) const
{
    if (std::max(v1, v2) >= getNumVertices())
    {
        throw MyException("UndirectedGraph::areAdjacent invalid vertex");
    }

    auto [itr, found] = edge(vertex(v1, m_graph), vertex(v2, m_graph), m_graph);
    return found;
}

std::vector<Vertex> UndirectedGraph::getIsolatedVertices() const
{
    std::vector<Vertex> result;
    for (Vertex v = 0; v < getNumVertices(); ++v)
    {
        if (getDegree(v) == 0)
            result.push_back(v);
    }
    return result;
}
UndirectedGraph UndirectedGraph::CreatePermuted(const IGraphUs &graph, std::initializer_list<Permutation::Entry> permut)
{
    return CreatePermuted(graph, Permutation::Create(permut));
}

UndirectedGraph UndirectedGraph::CreatePermuted(const IGraphUs &graph, const Permutation &permut)
{
    const auto nVertices = graph.getNumVertices();
    MyAssert(nVertices == permut.getCardinality());
    UndirectedGraph result(nVertices);
    std::vector<Vertex> ngbs;
    for (Vertex v0 = 0; v0 < nVertices; ++v0)
    {
        const auto v1 = permut(v0);
        graph.setAdjacentVertices(v0, ngbs);
        for (auto n0 : ngbs)
        {
            const auto n1 = permut(n0);
            if (n1 > v1) // add once
            {
                result.addEdge(n1, v1);
            }
        }
    }

    return result;
}

std::vector<Vertex> UndirectedGraph::getSortedDegreeSequence() const
{
    auto result = getDegreeSequence();
    str::sort(result);
    return result;
}

std::string UndirectedGraph::toString() const
{
    const auto nVertices = getNumVertices();
    std::stringstream retval;
    retval << "#vertices " << nVertices << "\n";
    std::vector<Vertex> ngbs;
    for (Vertex v = 0; v < nVertices - 1; ++v)
    {
        setAdjacentVertices(v, ngbs);
        for (auto ngb : ngbs)
        {
            if (v < ngb)
            {
                retval << v << " <-> " << ngb << "\n";
            }
        }
    }
    return retval.str();
}

UndirectedGraph UndirectedGraph::CreateComplement(const IGraphUs &graph)
{
    const auto nVertices = graph.getNumVertices();
    UndirectedGraph result(nVertices, "Complement of: " + graph.getName());
    if (nVertices <= 1)
    {
        return result;
    }

    std::vector<Vertex> vertices(nVertices);
    str::iota(vertices, 0);
    std::vector<Vertex> ngbs;
    std::vector<Vertex> newEdges;
    for (Vertex v = 0; v < nVertices; ++v)
    {
        graph.setAdjacentVertices(v, ngbs);
        newEdges.clear();
        std::set_difference(vertices.begin() + v + 1, vertices.end(), ngbs.begin(), ngbs.end(),
                            std::back_inserter(newEdges));
        for (Vertex n : newEdges)
        {
            result.addEdge(v, n);
        }
    }
    return result;
}

std::string UndirectedGraph::getName() const
{
    return m_name;
}

UndirectedGraph UndirectedGraph::CreateEdgesOmitted(const IGraphUs &graph,
                                                    const std::vector<std::vector<Vertex>> &omitEdgesBetween)
{
    UndirectedGraph result(graph.getNumVertices());
    std::map<Vertex, size_t> vertex2group;

    for (size_t group = 0; group < omitEdgesBetween.size(); ++group)
    {
        for (Vertex v : omitEdgesBetween.at(group))
        {
            if (!vertex2group.contains(v))
            {
                vertex2group[v] = group;
            }
            else
            {
                MyAssert(vertex2group.at(v) == group);
            }
        }
    }

    for (auto edge : graph.getAllSortedEdges())
    {
        bool doAdd = true;
        if (vertex2group.contains(edge[0]) && vertex2group.contains(edge[1]))
        {
            doAdd = (vertex2group[edge[0]] != vertex2group[edge[1]]);
        }
        if (doAdd)
        {
            result.addEdge(edge);
        }
    }
    return result;
}

UndirectedGraph UndirectedGraph::CreateEdgesKeep(const IGraphUs &graph, const std::vector<std::vector<Vertex>> &groups)
{
    UndirectedGraph result(graph.getNumVertices());
    std::map<Vertex, size_t> vertex2group;

    for (size_t group = 0; group < groups.size(); ++group)
    {
        for (Vertex v : groups.at(group))
        {
            MyAssert(!vertex2group.contains(v));
            vertex2group[v] = group;
        }
    }

    MyAssert(vertex2group.size() == graph.getNumVertices(),
             "UndirectedGraph::CreateEdgesKeep() all vertices must be in a group");

    for (auto edge : graph.getAllSortedEdges())
    {
        if (vertex2group.at(edge[0]) == vertex2group.at(edge[1]))
        {
            result.addEdge(edge);
        }
    }

    return result;
}

UndirectedGraph UndirectedGraph::CreateRandomShuffled(const IGraphUs &graph, Permutation::Entry seed)
{
    const auto nVertices = graph.getNumVertices();
    const Permutation trivial = Permutation::CreateTrivial(nVertices);
    const auto permutation = Permutation::CreateRandomShuffle(trivial, seed);
    return UndirectedGraph::CreatePermuted(graph, permutation);
}
