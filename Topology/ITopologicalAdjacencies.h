#pragma once

#include "Topology/TopologyDimension.h"
class ITopologicalAdjacency;

class ITopologicalAdjacencies
{
public:
   virtual ~ITopologicalAdjacencies() = default;
   virtual TopologyDimension getMaxTopologyDimension() const = 0;

   // Throws if dim > getMaxTopologyDimension()
   virtual int getCount(TopologyDimension dim) const = 0;

   // Should throw if hi == lo
   // Adjacency need not be available
   virtual std::pair<bool, const ITopologicalAdjacency*> getAdjacency(TopologyDimension hi, TopologyDimension lo) const = 0;
};