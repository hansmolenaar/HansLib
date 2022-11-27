#pragma once

#include "Topology/IReferenceShape.h"

class ReferenceshapePolyline : public IReferenceShape
{
public:
   static const ReferenceshapePolyline& get(int);
   const ITopologicalAdjacencies& getAdjacencies() const override;
};
