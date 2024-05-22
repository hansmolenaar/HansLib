#include "MeshingSettingsStandard.h"
#include "CellQuality2.h"
#include "RefinementPredicates.h"
#include "InitialBoundingboxGenerator.h"
#include "Ball2AsRegion.h"

using namespace MeshGeneration;

template MeshingSettingsStandard<2>;

template<int N>
MeshingSettingsStandard<N>::MeshingSettingsStandard(int maxLevel)
{
   const Geometry::Ball<GeomType, GeomDim2> ball(Point2{ 0.5, 0.5 }, 0.5);
   const Geometry::Ball2AsRegion<GeomType> ballAsRegion(ball);
   auto initialBbGenerator = InitialBoundingboxGenerator<GeomDim2>::Create(2.0);
   RefineRegionToMaxLevel<N> predicate(maxLevel, ballAsRegion, m_areClose, *initialBbGenerator);
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