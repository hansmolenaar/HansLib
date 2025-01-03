#pragma once

#include "TopologyDefines.h"
#include <optional>

class ITopologicalAdjacency;

class ITopologicalAdjacencies
{
public:
   virtual ~ITopologicalAdjacencies() = default;
   virtual Topology::TopologyDimension getMaxTopologyDimension() const = 0;

   // Throws if dim > getMaxTopologyDimension()
   virtual std::optional<int> getCount(Topology::TopologyDimension dim) const = 0;

   // dim1 and dim2 need not be ordered
   // Should throw if hi == lo
   // Should throw if dim1 > getMaxTopologyDimension() or dim2 > getMaxTopologyDimension()
   // Adjacency need not be available
   virtual std::optional<const ITopologicalAdjacency*> getAdjacency(Topology::TopologyDimension dim1, Topology::TopologyDimension dim2) const = 0;

   // Throws if not available, or dim > getMaxTopologyDimension()
   int getCountSafe(Topology::TopologyDimension dim) const;

   // counts and all adjacencies are available
   bool isComplete() const;
};