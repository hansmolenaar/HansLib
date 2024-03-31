#pragma once

#include "TopologyDimension.h"
#include <vector>

class ITopologicalAdjacency
{
public:
   virtual ~ITopologicalAdjacency() = default;
   virtual Topology::TopologyDimension getDimensionLow() const = 0;
   virtual Topology::TopologyDimension getDimensionHigh() const = 0;
   virtual const std::vector<int>& getConnectedLowers(int posHi) const = 0;
   virtual const std::vector<int>& getConnectedHighers(int posLo) const = 0;
};