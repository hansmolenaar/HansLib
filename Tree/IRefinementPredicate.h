#pragma once

#include "IntervalTree.h"
#include "IGeometryRegion.h"
#include "IInitialBoundingboxGenerator.h"

template<size_t N>
class IRefinementPredicate
{
public:
   ~IRefinementPredicate() noexcept = default;
   virtual bool operator()(const IntervalTree::Index<N>& indx) const = 0;
};


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
