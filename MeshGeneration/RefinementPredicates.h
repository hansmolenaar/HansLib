#pragma once
#include "IGeometryRegion.h"
#include "IntervalTreeIndex.h"
#include "IRefinementPredicate.h"

namespace MeshGeneration
{
   template<int N>
   class RefineRegionToMaxLevel : public IRefinementPredicate<N>
   {
   public:
      RefineRegionToMaxLevel(int maxLevel, const Geometry::IGeometryRegion<double, N>& region);
      bool operator()(const IntervalTree::Index<N>& indx) const override;

   private:
      int m_maxLevel;
      const Geometry::IGeometryRegion<double, N>& m_region;
   };


} // namespace MeshGeneration
