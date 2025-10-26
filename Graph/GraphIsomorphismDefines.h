#pragma once

#include <vector>

#include "GraphDefines.h"

class UndirectedGraph;

namespace GraphIsomorphism
{

using TagEntry = int;
using VertexTag = std::vector<TagEntry>;
using GraphGroup = std::vector<int>;
using GraphPair = std::pair<const UndirectedGraph &, const UndirectedGraph &>;
using VertexPair = std::pair<GraphVertex, GraphVertex>;

enum Status
{
    Undecided,
    Isomorphic,
    NotIsomorphic
};

} // namespace GraphIsomorphism
