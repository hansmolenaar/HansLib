#pragma once
#include "IGeometryRegion.h"

namespace MeshGeneration
{
   template<int N>
   struct RefineRegionToMaxLevel
   {
      bool operator()(const Index<N>& indx) const;
      int MaxLevel;
      const IGeometryRegion& Region;
   };


} // namespace MeshGeneration
