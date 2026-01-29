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
/*
        1
      / |
  3 -2  |
      \ |
        0
*/

static constexpr char house[] = "Dd[";
/*
          2
       /     \
      3 ------ 4
      |        |
      0 ------ 1
*/

static constexpr char fish[] = "ErCW";
/*
            1    4
          /  \  /|
         0    3  |
          \  / \ |
            2    5
*/

static constexpr char X84[] = "ElD?";
/*
            0
          /  \
     5 - 1    3 - 4
          \  /
            2
*/

static constexpr char X197[] = "EgC";
/*
            5
         3 ---4
       0 -- 1 -- 2
*/

static constexpr char butterfly[] = "D{c";
/*
            1    4
            |\  /|
            | 0  |
            |/ \ |
            2    3
*/

static constexpr char diamond[] = "Cz";
/*
            1
          / |\
         0  | 3
          \ |/
            2
*/

const std::vector<std::string> &getListNumVertices_5_connected();
const std::vector<std::string> &getListNumVertices_6_connected();
const std::vector<std::string> &getListNumVertices_7_connected();
const std::vector<std::string> &getListNumVertices_8_connected();
const std::vector<std::string> &getListNumVertices_11_connected();
const std::vector<std::string> &getListNumVertices_12_connected();

// Mix of connected and disconnected matrices
const std::vector<std::string> &getListNumVertices_3();
const std::vector<std::string> &getListNumVertices_4();
const std::vector<std::string> &getListNumVertices_5();
const std::vector<std::string> &getListNumVertices_6();
const std::vector<std::string> &getListNumVertices_7();
const std::vector<std::string> &getListNumVertices_8();
const std::vector<std::string> &getListNumVertices_9();
const std::vector<std::string> &getListNumVertices_10();

std::vector<std::unique_ptr<Graph::IGraphUs>> getDisconnectedGraphs(const std::vector<std::string> &);
std::vector<std::unique_ptr<Graph::IGraphUs>> getGraphs(const std::vector<std::string> &);

} // namespace UndirectedGraphFromG6
