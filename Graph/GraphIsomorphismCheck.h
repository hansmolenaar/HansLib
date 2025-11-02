#pragma once

#include "GraphIsomorphismDefines.h"
#include "IGraphUS.h"

namespace GraphIsomorphism
{
struct Check
{
    bool operator()(const Graph::IGraphUS &, const Graph::IGraphUS &) const;
    bool operator()(const Graph::IGraphUS &, const std::vector<VertexPair> &, const Graph::IGraphUS &) const;
};
} // namespace GraphIsomorphism
