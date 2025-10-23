#pragma once

#include "GraphIsomorphismDefines.h"
#include "UndirectedGraph.h"

namespace GraphIsomorphism
{
struct Check
{
    Status operator()(const UndirectedGraph &g0, const UndirectedGraph &g1);
};
} // namespace GraphIsomorphism
