#pragma once

#include "Point.h"

template<typename T, int N>
class IGeometryPredicate
{
public:
   virtual ~IGeometryPredicate() = default;
   virtual bool SamePoints(Point<T, N> p0, Point<T, N> p1) const = 0;
   virtual double getSmallLengthInDirection(int n) const = 0;
};
