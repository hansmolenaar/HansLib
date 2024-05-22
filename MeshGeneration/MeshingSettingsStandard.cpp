#include "MeshingSettingsStandard.h"
#include "CellQuality2.h"
#include "RefinementPredicates.h"
#include "InitialBoundingboxGenerator.h"
#include "Ball2AsRegion.h"

using namespace MeshGeneration;

template MeshingSettingsStandard<2>;

template<int N>
MeshingSettingsStandard<N>::MeshingSettingsStandard(const Geometry::IGeometryRegion<double, N>& region, int maxLevel, double initBbMultiplier) :
   m_refinementPredicateFactory(maxLevel)
{
   m_bbGenerator = InitialBoundingboxGenerator<N>::Create(initBbMultiplier);
   m_refinementPredicate = m_refinementPredicateFactory.Create(region, *m_bbGenerator, m_areClose);
   m_strategy = std::make_unique<MeshingStrategy2>(*m_bbGenerator, *m_refinementPredicate);
}

template<int N>
MeshingStrategy<N>& MeshingSettingsStandard<N>::getStrategy()
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

template<int N>
IRefinementPredicateFactory<N>& MeshingSettingsStandard<N>::getRefinementPredicateFactory()
{
   return m_refinementPredicateFactory;
}

template<int N>
const IGeometryPredicate<double, N>& MeshingSettingsStandard<N>::getGeometryPredicate()
{
   return m_areClose;
}

template<int N>
const IInitialBoundingboxGenerator<N >& MeshingSettingsStandard<N>::getInitialBbGenerator()
{
   return *m_bbGenerator;
}