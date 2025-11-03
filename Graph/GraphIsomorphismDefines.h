#pragma once

#include <vector>

#include "GraphDefines.h"

namespace Graph
{
class UndirectedGraph;
}

namespace GraphIsomorphism
{

using TagEntry = int;
using Tag = std::vector<TagEntry>;
using GraphGroup = std::vector<int>;
using GraphPair = std::pair<const Graph::UndirectedGraph &, const Graph::UndirectedGraph &>;
using VertexPair = std::pair<GraphVertex, GraphVertex>;

enum Flag
{
    Undecided,
    Isomorphic,
    NotIsomorphic
};

} // namespace GraphIsomorphism
