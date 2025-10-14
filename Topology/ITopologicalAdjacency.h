#pragma once

#include "TopologyDefines.h"
#include <vector>

namespace Topology
{
   class ITopologicalAdjacency
   {
   public:
      virtual ~ITopologicalAdjacency() = default;
      virtual TopologyDimension getDimensionLow() const = 0;
      virtual TopologyDimension getDimensionHigh() const = 0;
      virtual const std::vector<int>& getConnectedLowers(int posHi) const = 0;
      virtual const std::vector<int>& getConnectedHighers(int posLo) const = 0;
   };
}