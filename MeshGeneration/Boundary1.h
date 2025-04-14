#pragma once

#include "EdgeNodesSorted.h"
#include "TrianglesNodes.h"

namespace MeshGeneration
{
   class Boundary1
   {
   public:
      Boundary1() = default;

      Boundary1(std::span<const Topology::NodeIndex> activeNodes, const TrianglesNodes& trianglesNodes);

      std::vector<Topology::NodeIndex> Singletons;
      std::vector<std::vector<Topology::NodeIndex>> Cycles;
      std::vector<std::vector<Topology::NodeIndex>> Paths;
   };

}