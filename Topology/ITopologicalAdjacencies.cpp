#include "ITopologicalAdjacencies.h"
#include "MyAssert.h"

int ITopologicalAdjacencies::getCountSafe(TopologyDimension dim) const
{
   const auto result = getCount(dim);
   Utilities::MyAssert(result.has_value());
   return *result;
}

bool ITopologicalAdjacencies::isComplete() const
{
   for (TopologyDimension dim = TopologyDimensionDef::Corner; dim <= getMaxTopologyDimension(); ++dim)
   {
      if (!getCount(dim)) return false;
   }

   for (TopologyDimension dimH = TopologyDimensionDef::Edge; dimH <= getMaxTopologyDimension(); ++dimH)
   {
      for (TopologyDimension dimL = TopologyDimensionDef::Corner; dimL < dimH; ++dimL)
      {
         if (!getAdjacency(dimL, dimH)) return false;
      }
   }
   return true;
}