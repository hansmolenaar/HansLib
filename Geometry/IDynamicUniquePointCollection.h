#pragma once

#include "IUniquePointCollection.h"

template<typename T, size_t N>
class IDynamicUniquePointCollection : public IUniquePointCollection<T, N>
{
public:
   virtual PointIndex addIfNew(const Point<double, N>& point) = 0;
   virtual void deletePoint(PointIndex pointId) = 0;
   virtual void movePoint(PointIndex pointId, const Point<double, N>& newLocation) = 0;
};

using IDynamicUniquePointCollection2 = IDynamicUniquePointCollection<double, GeomDim2>;
using IDynamicUniquePointCollection3 = IDynamicUniquePointCollection<double, GeomDim3>;
