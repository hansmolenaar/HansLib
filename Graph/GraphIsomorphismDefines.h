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
using GraphTags = std::vector<Tag>;
using VertexTags = std::vector<Tag>;

enum Flag
{
    Undecided,
    Isomorphic,
    NotIsomorphic
};

} // namespace GraphIsomorphism
