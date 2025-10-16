#pragma once

#include "Point.h"

template<typename T, size_t N>
class IPointCollection
{
public:
   ~IPointCollection() noexcept = default;

   virtual Point<T, N> getPoint(PointIndex) const = 0;
   virtual PointIndex getNumPoints() const = 0;
};

using IPointCollection2 = IPointCollection<double, GeomDim2>;
using IPointCollection3 = IPointCollection<double, GeomDim3>;
