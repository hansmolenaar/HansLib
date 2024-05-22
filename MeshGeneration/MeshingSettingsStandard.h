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
      IRefinementPredicateFactory<N>& getRefinementPredicateFactory() override;
      Logger& getLogger() override;
      const IGeometryPredicate<double, N>& getGeometryPredicate() override;
      const IInitialBoundingboxGenerator<N >& getInitialBbGenerator() override;

   private:
      PointClose<GeomType, N> m_areClose;
      std::unique_ptr<MeshingStrategy<N>> m_strategy;
      RefineRegionToMaxLevelFactory<N> m_refinementPredicateFactory;
      std::unique_ptr<IRefinementPredicate<N>> m_refinementPredicate;
      std::unique_ptr<IInitialBoundingboxGenerator<N>> m_bbGenerator;
      Logger m_logger;
   };

}