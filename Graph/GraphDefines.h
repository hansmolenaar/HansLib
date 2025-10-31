#pragma once

#include "boost/graph/adjacency_list.hpp"
#include <boost/graph/connected_components.hpp>
#include <boost/graph/sequential_vertex_coloring.hpp>

using GraphVertex = size_t;
using GraphEdge = size_t;

// Undirected graph, no data
using UGraph = boost::adjacency_list<boost::listS, boost::vecS, boost::undirectedS>;

using UGraphVertex = boost::graph_traits<UGraph>::vertex_descriptor;
using UGraphEdge = boost::graph_traits<UGraph>::edge_descriptor;

inline UGraphVertex UGraphAddVertex(UGraph &ug)
{
    return boost::add_vertex(ug);
}

inline void UGraphAddEdge(UGraph &ug, UGraphVertex vertex1, UGraphVertex vertex2)
{
    boost::add_edge(vertex1, vertex2, ug);
}

inline size_t UGraphNumVertices(const UGraph &ug)
{
    return boost::num_vertices(ug);
}

inline std::tuple<std::vector<int>, size_t> UGraphGetConnectedComponents(const UGraph &ug)
{
    std::vector<int> result(UGraphNumVertices(ug));
    const auto numComponentes = boost::connected_components(ug, result.data());
    return {result, numComponentes};
}
