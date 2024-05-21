#include "MeshingSettings.h"

using namespace MeshGeneration;

template MeshingSettings<2>;
template MeshingSettings<3>;

template<int N>
MeshingSettings<N>::MeshingSettings(std::unique_ptr<MeshingStrategy<N>>&& strategy /* , MeshGeneration::CellQuality<N> quality */ ) :
   m_strategy(std::move(strategy)) //, m_quality(quality))
{
   // Empty
}

template<int N>
const MeshingStrategy<N>& MeshingSettings<N>::getStrategy() const
{
   return *m_strategy;
}

#if false
template<int N>
const MeshGeneration::CellQuality<N>& MeshingSettings<N>::getMeshQuality() const
{
   return m_quality;
}
#endif

template<int N>
Logger& MeshingSettings<N>::getLogger()
{
   return m_logger;
}