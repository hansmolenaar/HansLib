#pragma once

#include "IReferenceShape.h"

class ReferenceShapeNode : public IReferenceShape
{
public:
   TopologyDimension getMaxTopologyDimension() const override;
   int getCount(TopologyDimension dim) const override;
   const ITopologicalAdjacency& getAdjacency(TopologyDimension hi, TopologyDimension lo) const override;
};
