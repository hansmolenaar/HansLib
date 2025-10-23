#pragma once

#include "GraphIsomorphismStatus.h"
#include "UndirectedGraph.h"

namespace GraphIsomorphism
{
struct Check
{
    Status operator()(const UndirectedGraph &g0, const UndirectedGraph &g1);
};
} // namespace GraphIsomorphism
