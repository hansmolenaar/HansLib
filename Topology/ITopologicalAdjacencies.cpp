#include "ITopologicalAdjacencies.h"
#include "Utilities/Assert.h"

int ITopologicalAdjacencies::getCountSafe(TopologyDimension dim) const
{
   const auto result = getCount(dim);
   Utilities::Assert(result.first);
   return result.second;
}