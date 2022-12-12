#pragma once

#include "Point/Point.h"

template<typename T, int N>
class IPointCollection
{
public:
   ~IPointCollection() = default;

   virtual Point<T, N> getPoint(PointIndex) const = 0;
   virtual int getNumPoints() const = 0;
};
