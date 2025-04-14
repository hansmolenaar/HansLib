#pragma once

#include "EdgeNodesSorted.h"
#include "TrianglesNodes.h"

namespace MeshGeneration
{
   class Boundary1
   {
   public:
      Boundary1() = default;

      explicit Boundary1(const TrianglesNodes& trianglesNodes);
      Boundary1(std::span<const Topology::NodeIndex> activeNodes, const TrianglesNodes& trianglesNodes);
      static Boundary1 createSingleCycleForTesting(const std::vector<Topology::NodeIndex>& cycle);
      const std::vector<Topology::NodeIndex>& getSingletons() const;
      const std::vector<std::vector<Topology::NodeIndex>>& getPaths() const;
      const std::vector<std::vector<Topology::NodeIndex>>& getCycles() const;
      bool empty() const;

   private:
      explicit Boundary1(const std::vector<Topology::NodeIndex>& cycle);

      std::vector<Topology::NodeIndex> m_singletons;
      std::vector<std::vector<Topology::NodeIndex>> m_paths;
      std::vector<std::vector<Topology::NodeIndex>> m_cycles;
   };

}