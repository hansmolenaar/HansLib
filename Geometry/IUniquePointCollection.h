#pragma once

#include "IPointCollection.h"
#include "IGeometryPredicate.h"

template<typename T, int N>
class IUniquePointCollection : public IPointCollection<T,N>
{
public:
   virtual std::tuple<bool, PointIndex>  tryGetClosePoint(const Point<T, N>&) const = 0;
   virtual const IGeometryPredicate<T,N>& getGeometryPredicate() const = 0;
};