#pragma once

#include "GraphIsomorphismDefines.h"
#include "UndirectedGraph.h"

namespace GraphIsomorphism
{
struct Check
{
    Flag operator()(const UndirectedGraph &, const UndirectedGraph &) const;
    Flag operator()(const UndirectedGraph &, const std::vector<VertexPair> &, const UndirectedGraph &) const;
};
} // namespace GraphIsomorphism
