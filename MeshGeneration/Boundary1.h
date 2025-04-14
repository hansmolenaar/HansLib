#pragma once

#include "EdgeNodesSorted.h"
#include "TrianglesNodes.h"

namespace MeshGeneration
{
   class Boundary1
   {
   public:
      Boundary1() = default;

      static Boundary1 createFromSubSet(const std::vector<Topology::NodeIndex>& activeNodes, const TrianglesNodes& trianglesNodes);
      static Boundary1 createFromBoundaryEdges(const TrianglesNodes& trianglesNodes);
      const std::vector<Topology::NodeIndex>& getSingletons() const;
      const std::vector<std::vector<Topology::NodeIndex>>& getPaths() const;
      const std::vector<std::vector<Topology::NodeIndex>>& getCycles() const;
      bool empty() const;

   private:
      explicit Boundary1(const std::vector<Topology::EdgeNodesSorted>& edgeSet, std::vector<Topology::NodeIndex> xNodes = {});

      std::vector<Topology::NodeIndex> m_singletons;
      std::vector<std::vector<Topology::NodeIndex>> m_paths;
      std::vector<std::vector<Topology::NodeIndex>> m_cycles;
   };

}