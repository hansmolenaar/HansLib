#pragma once

#include "Topology/TopologyDimension.h"
class ITopologicalAdjacency;

class ITopologicalAdjacencies
{
public:
   virtual ~ITopologicalAdjacencies() = default;
   virtual TopologyDimension getMaxTopologyDimension() const = 0;
   virtual int getCount(TopologyDimension dim) const = 0;
   virtual const ITopologicalAdjacency& getAdjacency(TopologyDimension hi, TopologyDimension lo) const = 0;
};