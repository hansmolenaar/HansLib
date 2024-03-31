#pragma once

#include "IInitialBoundingboxGenerator.h"
#include "IRefinementPredicate.h"

#include <memory>

template<int N>
class MeshingStrategy
{
public:
   MeshingStrategy(const IInitialBoundingboxGenerator<N>& initialBoundingBoxGenerator, const IRefinementPredicate<N>& refinementPredicate);
   const IInitialBoundingboxGenerator<N>& getInitialBoundingBoxGenerator() const;
   const IRefinementPredicate<N>& getRefinementPredicate() const;

private:
   const IInitialBoundingboxGenerator<N>& m_initialBbGenerator;
   const IRefinementPredicate<N>& m_refinementPredicate;
};

using MeshingStrategy2 = MeshingStrategy<GeomDim2>;
using MeshingStrategy3 = MeshingStrategy<GeomDim3>;