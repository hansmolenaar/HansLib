#include "MeshingStrategy.h"

template MeshingStrategy<2>;
template MeshingStrategy<3>;

template<int N>
MeshingStrategy<N>::MeshingStrategy(const IInitialBoundingboxGenerator<N>& initialBoundingBoxGenerator, const IRefinementPredicate<N>& refinementPredicate) : 
   m_initialBbGenerator(initialBoundingBoxGenerator),
   m_refinementPredicate(refinementPredicate)
{}

template<int N>
const IInitialBoundingboxGenerator<N>& MeshingStrategy<N>::getInitialBoundingBoxGenerator() const
{
   return m_initialBbGenerator;
}

template<int N>
const IRefinementPredicate<N>& MeshingStrategy<N>::getRefinementPredicate() const
{
   return m_refinementPredicate;
}