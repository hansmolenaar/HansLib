#pragma once

#include <vector>

#include "GraphDefines.h"

namespace GraphIsomorphism
{

using VertexTag = std::vector<int>;
using GraphGroup = std::vector<int>;

enum Status
{
    Undecided,
    Isomorphic,
    NotIsomorphic
};

} // namespace GraphIsomorphism
