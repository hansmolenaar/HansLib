#pragma once

#include "IMeshingSettings.h"
#include "PointClose.h"
#include "RefinementPredicates.h"

namespace MeshGeneration
{
   template<int N>
   class MeshingSettingsStandard : public IMeshingSettings<N>
   {
   public:
      MeshingSettingsStandard(const Geometry::IGeometryRegion<double, N>& region, int maxLevel, double initBbMultiplier);

      MeshingStrategy<N>& getStrategy()  override;
      MeshGeneration::CellQualityFun<N>* getMeshQuality()  override;
      Logger& getLogger() override;

   private:
      PointClose<GeomType, GeomDim2> m_areClose;
      std::unique_ptr<MeshingStrategy<N>> m_strategy;
      std::unique_ptr<RefineRegionToMaxLevel<N>> m_refinementPredicate;
      Logger m_logger;
   };

}