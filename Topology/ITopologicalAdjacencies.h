#pragma once

#include "Topology/TopologyDimension.h"
class ITopologicalAdjacency;

class ITopologicalAdjacencies
{
public:
   virtual ~ITopologicalAdjacencies() = default;
   virtual TopologyDimension getMaxTopologyDimension() const = 0;

   // Throws if dim > getMaxTopologyDimension()
   virtual std::pair<bool, int> getCount(TopologyDimension dim) const = 0;

   // dim1 and dim2 need not be ordered
   // Should throw if hi == lo
   // Should throw if dim1 > getMaxTopologyDimension() or dim2 > getMaxTopologyDimension()
   // Adjacency need not be available
   virtual std::pair<bool, const ITopologicalAdjacency*> getAdjacency(TopologyDimension dim1, TopologyDimension dim2) const = 0;

   int getCountSafe(TopologyDimension dim) const;
};