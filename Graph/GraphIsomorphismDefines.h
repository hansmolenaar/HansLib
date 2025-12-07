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
using GraphTags = std::vector<Tag>;
using VertexTags = std::vector<Tag>;

using GraphGroup = std::vector<int>;
using GraphPair = std::pair<const Graph::UndirectedGraph &, const Graph::UndirectedGraph &>;
using VertexPair = std::array<GraphVertex, 2>;
using OrderedNonLoopEdge = VertexPair; // Not checked, responsibility of creator

enum Flag
{
    Undecided,
    Isomorphic,
    NotIsomorphic
};

} // namespace GraphIsomorphism
