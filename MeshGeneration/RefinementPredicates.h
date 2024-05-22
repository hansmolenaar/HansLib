#pragma once
#include "IGeometryRegion.h"
#include "IntervalTreeIndex.h"
#include "IRefinementPredicate.h"
#include "IInitialBoundingboxGenerator.h"

namespace MeshGeneration
{
   template<int N>
   class RefineRegionToMaxLevel : public IRefinementPredicate<N>
   {
   public:
      RefineRegionToMaxLevel(int maxLevel, const Geometry::IGeometryRegion<double, N>& region, const IGeometryPredicate<double, N>& predicate,
         const IInitialBoundingboxGenerator<N>& generator);
      bool operator()(const IntervalTree::Index<N>& indx) const override;

   private:
      int m_maxLevel;
      const Geometry::IGeometryRegion<double, N>& m_region;
      BoundingBox<double, N> m_initialBb;
      const IGeometryPredicate<double, N>& m_geometryPredicate;
   };


   template<int N>
   class RefineRegionToMaxLevelFactory : public IRefinementPredicateFactory<N>
   {
   public:
      explicit RefineRegionToMaxLevelFactory<N>(int maxLevel);
      std::unique_ptr<IRefinementPredicate<N>> Create(
         const Geometry::IGeometryRegion<double, N>& region, 
         const IInitialBoundingboxGenerator<N>& generator, 
         const IGeometryPredicate<double, N>& geometryPredicate) override;

   private:
      int m_maxLevel;
   };

} // namespace MeshGeneration
