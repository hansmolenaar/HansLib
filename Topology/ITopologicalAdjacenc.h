#pragma once

#include "Topology/TopologyDimension.h"
#include <vector>

class ITopologicalAdjacency
{
   virtual ~ITopologicalAdjacency() = default;
   virtual TopologyDimension getDimensionLow() const = 0;
   virtual  TopologyDimension getDimensionHigh() const = 0;
   virtual const std::vector<int> getConnectedLowers(int posHi) const = 0;
   virtual  const std::vector<int> getConnectedHighers(int posHi) const = 0;
   //IEnumerable<int> GetAllLowers();
   //IEnumerable<int> GetAllHighers();
   //bool IsActiveHigh(int posHi);
   //bool IsActiveLow(int posLo);
}