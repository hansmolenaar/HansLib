#include "MeshingSettingsStandard.h"
#include "CellQuality2.h"
#include "RefinementPredicates.h"
#include "InitialBoundingboxGenerator.h"
#include "Ball2AsRegion.h"

using namespace MeshGeneration;

template class MeshingSettingsStandard<2>;

template<size_t N>
MeshingSettingsStandard<N>::MeshingSettingsStandard(int maxLevel, double initBbMultiplier) :
   m_refinementPredicateFactory(maxLevel), m_bbGenerator(InitialBoundingboxGenerator<N>::Create(initBbMultiplier))
{
}

template<size_t N>
CellQualityFun<N>* MeshingSettingsStandard<N>::getCellQuality()
{
   return CellQuality2::MinimumAngle;
}


template<size_t N>
Logger& MeshingSettingsStandard<N>::getLogger()
{
   return m_logger;
}

template<size_t N>
IRefinementPredicateFactory<N>& MeshingSettingsStandard<N>::getRefinementPredicateFactory()
{
   return m_refinementPredicateFactory;
}

template<size_t N>
const IGeometryPredicate<double, N>& MeshingSettingsStandard<N>::getGeometryPredicate()
{
   return m_areClose;
}

template<size_t N>
const IInitialBoundingboxGenerator<N >& MeshingSettingsStandard<N>::getInitialBbGenerator()
{
   return *m_bbGenerator;
}
