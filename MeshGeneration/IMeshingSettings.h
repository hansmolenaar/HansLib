#pragma once

#include "MeshGenerationDefines.h"
#include "RefinementPredicates.h"
#include "IGeometryRegion.h"
#include "Logger.h"

namespace MeshGeneration
{
   template<int N>
   class IMeshingSettings
   {
   public:
      virtual ~IMeshingSettings() = default;

      virtual MeshGeneration::CellQualityFun<N>* getCellQuality() = 0;
      virtual IRefinementPredicateFactory<N>& getRefinementPredicateFactory() = 0;
      virtual const IGeometryPredicate<double, N>& getGeometryPredicate() = 0;
      virtual const IInitialBoundingboxGenerator<N >& getInitialBbGenerator() = 0;
      virtual Logger& getLogger() = 0;

      BoundingBox<GeomType, N> getInitialBb(const Geometry::IGeometryRegion<GeomType, N>& region);
   };

   using IMeshingSettings2 = IMeshingSettings<2>;
}