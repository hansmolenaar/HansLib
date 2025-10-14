#pragma once

#include "Point.h"

template<typename T, size_t N>
class IGeometryPredicate
{
public:
   virtual ~IGeometryPredicate() = default;
   virtual bool samePoints(Point<T, N> p0, Point<T, N> p1) const = 0;
   virtual T getSmallLengthInDirection(int n) const = 0;
   virtual T getSmallNormSquared() const = 0;
};
