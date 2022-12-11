#include "ITopologicalAdjacencies.h"
#include "Utilities/Assert.h"

int ITopologicalAdjacencies::getCountSafe(TopologyDimension dim) const
{
   const auto result = getCount(dim);
   Utilities::Assert(result.first);
   return result.second;
}

bool ITopologicalAdjacencies::isComplete() const
{
   for (TopologyDimension dim = TopologyDimensionDef::Corner; dim <= getMaxTopologyDimension(); ++dim)
   {
      if (!getCount(dim).first) return false;
   }

   for (TopologyDimension dimH = TopologyDimensionDef::Edge; dimH <= getMaxTopologyDimension(); ++dimH)
   {
      for (TopologyDimension dimL = TopologyDimensionDef::Corner; dimL < dimH; ++dimL)
      {
         if (!getAdjacency(dimL, dimH).first) return false;
      }
   }
   return true;
}