#pragma once

#include "IMeshingSettings.h"
#include "PointClose.h"

namespace MeshGeneration
{
   template<int N>
   class MeshingSettingsStandard : public IMeshingSettings<N>
   {
   public:
      MeshingSettingsStandard(const Geometry::IGeometryRegion<double, N>& region, int maxLevel);
      //static std::unique_ptr<MeshingSettingsStandard<2>> CreateDefault2();

      const MeshingStrategy<N>& getStrategy() const override;
      MeshGeneration::CellQualityFun<N>* getMeshQuality()  override;
      Logger& getLogger() override;

   private:
      PointClose<GeomType, GeomDim2> m_areClose;
      std::unique_ptr<MeshingStrategy<N>> m_strategy;
      Logger m_logger;
   };

}