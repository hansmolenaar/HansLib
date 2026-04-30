#pragma once

#include "GraphIsomorphismDefines.h"
#include "IGraphUs.h"

namespace GraphIsomorphism
{
struct Check
{
    bool operator()(const Graph::IGraphUs &, const Graph::IGraphUs &) const;
    bool operator()(const Graph::IGraphUs &, const std::vector<Graph::VertexPair> &, const Graph::IGraphUs &) const;
};
} // namespace GraphIsomorphism
