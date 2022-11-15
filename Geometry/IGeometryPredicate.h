#pragma once

#include "Point/Point.h"

template<typename T, int N>
class IGeometryPredicate
{
public:
   virtual ~IGeometryPredicate() = default;
   virtual bool SamePoints(Point<T, N> p0, Point<T, N> p1) const = 0;
};
