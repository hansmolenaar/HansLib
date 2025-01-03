#include "ITopologicalAdjacencies.h"
#include "MyAssert.h"

using namespace Topology;

int ITopologicalAdjacencies::getCountSafe(TopologyDimension dim) const
{
   const auto result = getCount(dim);
   Utilities::MyAssert(result.has_value());
   return *result;
}

bool ITopologicalAdjacencies::isComplete() const
{
   for (TopologyDimension dim = Topology::Corner; dim <= getMaxTopologyDimension(); ++dim)
   {
      if (!getCount(dim)) return false;
   }

   for (TopologyDimension dimH = Topology::Edge; dimH <= getMaxTopologyDimension(); ++dimH)
   {
      for (TopologyDimension dimL = Topology::Corner; dimL < dimH; ++dimL)
      {
         if (!getAdjacency(dimL, dimH)) return false;
      }
   }
   return true;
}