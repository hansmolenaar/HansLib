#pragma once

#include "Point.h"

template<typename T, int N>
class IPointCollection
{
public:
   ~IPointCollection() = default;

   virtual Point<T, N> getPoint(PointIndex) const = 0;
   virtual PointIndex getNumPoints() const = 0;
};
