#pragma once

#include "IInitialBoundingboxGenerator.h"
#include "IGeometryRegion.h"

template<size_t N>
class IRefinementPredicateFactory
{
public:
   ~IRefinementPredicateFactory() noexcept = default;
   virtual std::unique_ptr<IRefinementPredicate<N>> Create(
      const Geometry::IGeometryRegion<double, N>& region, 
      const IInitialBoundingboxGenerator<N>& generator,
      const IGeometryPredicate<double, N>& geometryPredicate) = 0;
};
