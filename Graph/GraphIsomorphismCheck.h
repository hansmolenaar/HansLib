#pragma once

#include "GraphIsomorphismDefines.h"
#include "UndirectedGraph.h"

namespace GraphIsomorphism
{
struct Check
{
    Status operator()(const UndirectedGraph &, const UndirectedGraph &) const;
    Status operator()(const UndirectedGraph &, const std::vector<VertexPair> &, const UndirectedGraph &) const;
};
} // namespace GraphIsomorphism
