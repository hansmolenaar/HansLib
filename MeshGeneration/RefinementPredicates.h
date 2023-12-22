#pragma once
#include "IGeometryRegion.h"
#include "IntervalTreeIndex.h"

namespace MeshGeneration
{
   template<int N>
   struct RefineRegionToMaxLevel
   {
      bool operator()(const IntervalTree::Index<N>& indx) const;
      int MaxLevel;
      const Geometry::IGeometryRegion<double, N>& Region;
   };


} // namespace MeshGeneration
