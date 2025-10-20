#pragma once

#include "IMeshingSettings.h"
#include "PointClose.h"
#include "RefinementPredicates.h"

namespace MeshGeneration
{
template <size_t N> class MeshingSettingsStandard : public IMeshingSettings<N>
{
  public:
    MeshingSettingsStandard(int maxLevel, double initBbMultiplier);

    MeshGeneration::CellQualityFun<N> *getCellQuality() override;
    IRefinementPredicateFactory<N> &getRefinementPredicateFactory() override;
    Logger &getLogger() override;
    const IGeometryPredicate<double, N> &getGeometryPredicate() override;
    const IInitialBoundingboxGenerator<N> &getInitialBbGenerator() override;

  private:
    PointClose<GeomType, N> m_areClose;
    RefineRegionToMaxLevelFactory<N> m_refinementPredicateFactory;
    std::unique_ptr<IInitialBoundingboxGenerator<N>> m_bbGenerator;
    Logger m_logger;
};

} // namespace MeshGeneration
