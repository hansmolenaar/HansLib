#pragma once

#include "Topology/ITopologicalAdjacencies.h"

class IReferenceShape 
{
public:
   virtual ~IReferenceShape() = default;
   virtual const ITopologicalAdjacencies& getAdjacencies() const = 0;
};
