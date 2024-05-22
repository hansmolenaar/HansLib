#include "MeshingSettingsStandard.h"
#include "CellQuality2.h"
#include "RefinementPredicates.h"
#include "InitialBoundingboxGenerator.h"
#include "Ball2AsRegion.h"

using namespace MeshGeneration;

template MeshingSettingsStandard<2>;

template<int N>
MeshingSettingsStandard<N>::MeshingSettingsStandard(const Geometry::IGeometryRegion<double, N>& region, int maxLevel)
{
   auto initialBbGenerator = InitialBoundingboxGenerator<GeomDim2>::Create(2.0);
   RefineRegionToMaxLevel<N> predicate(maxLevel, region, m_areClose, *initialBbGenerator);
   m_strategy = std::make_unique<MeshingStrategy2>(*initialBbGenerator, predicate);
}

template<int N>
const MeshingStrategy<N>& MeshingSettingsStandard<N>::getStrategy() const
{
   return *m_strategy;
}

template<int N>
CellQualityFun<N>* MeshingSettingsStandard<N>::getMeshQuality()
{
   return CellQuality2::MinimumAngle;
}


template<int N>
Logger& MeshingSettingsStandard<N>::getLogger()
{
   return m_logger;
}