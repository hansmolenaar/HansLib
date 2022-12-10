#pragma once

#include "Topology/ITopologicalAdjacencies.h"

class IReferenceShape 
{
public:
   virtual ~IReferenceShape() = default;

   // Count of maximum dimension should be 1
   // Should at least contain the max-Corner adjacency
   virtual const ITopologicalAdjacencies& getAdjacencies() const = 0;
};
