#pragma once

#include "GraphUsc.h"
#include "UndirectedGraph.h"

#include <memory>
#include <string>

// See https://www.graphclasses.org/smallgraphs.html

namespace UndirectedGraphFromG6
{
std::unique_ptr<Graph::UndirectedGraph> Create(std::string);
std::unique_ptr<Graph::GraphUsc> CreateConnected(std::string);

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

static constexpr char fish[] = "ErCW";
//
//
//          1    4
//        /  \  /|
//       0    3  |
//        \  / \ |
//          2    5

static constexpr char X84[] = "ElD?";
//
//
//          0
//        /  \
//   5 - 1    3 - 4
//        \  /
//          2
//

const std::vector<std::string> &getListNumVertices_5_connected();
const std::vector<std::string> &getListNumVertices_6_connected();
const std::vector<std::string> &getListNumVertices_7_connected();
const std::vector<std::string> &getListNumVertices_8_connected();

// Mix of connected and disconnected matrices
const std::vector<std::string> &getListNumVertices_5();
const std::vector<std::string> &getListNumVertices_6();

std::vector<std::unique_ptr<Graph::UndirectedGraph>> getDisconnectedGraphs(const std::vector<std::string> &);

} // namespace UndirectedGraphFromG6
