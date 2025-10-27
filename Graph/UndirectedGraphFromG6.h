#pragma once

#include "UndirectedGraph.h"

#include <memory>
#include <string>

// See https://www.graphclasses.org/smallgraphs.html

namespace UndirectedGraphFromG6
{
std::unique_ptr<UndirectedGraph> Create(std::string);

static constexpr char pan3[] = "Cx";
//      1
//    / |
// 3 -2  |
//    \ |
//      0

static constexpr char house[] = "Dd[";
//        2
//     /     \
//    3 ------ 4
//    |        |
//    0 ------ 1

} // namespace UndirectedGraphFromG6
