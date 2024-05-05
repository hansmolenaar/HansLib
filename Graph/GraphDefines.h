#pragma once

#include "boost/graph/adjacency_list.hpp"
#include <boost/graph/connected_components.hpp>
#include <boost/graph/sequential_vertex_coloring.hpp>

template<typename Vertex, typename Edge>
using Graph = boost::adjacency_list<boost::listS, boost::vecS, boost::undirectedS, Vertex, Edge >;