#pragma once

#include "UndirectedGraph.h"

#include <memory>
#include <string>

// See https://www.graphclasses.org/smallgraphs.html

namespace UndirectedGraphFromG6
{
std::unique_ptr<UndirectedGraph> Create(std::string);
}
