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

enum Flag
{
    Undecided,
    Isomorphic,
    NotIsomorphic
};

} // namespace GraphIsomorphism
