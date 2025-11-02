#pragma once

#include "GraphIsomorphismDefines.h"
#include "IGraphUSC.h"
#include "UndirectedGraph.h"

namespace GraphIsomorphism
{
struct Check
{
    bool operator()(const Graph::IGraphUS &, const Graph::IGraphUS &) const;
    bool operator()(const UndirectedGraph &, const std::vector<VertexPair> &, const UndirectedGraph &) const;
    bool operator()(const Graph::IGraphUSC &, const std::vector<VertexPair> &, const Graph::IGraphUSC &) const;
};
} // namespace GraphIsomorphism
