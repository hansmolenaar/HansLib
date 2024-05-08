#pragma once

#include "UndirectedGraph.h"

#include <memory>
#include <string>

namespace UndirectedGraphFromG6
{
   std::unique_ptr<UndirectedGraph> Create(std::string);
}