#pragma once

#include "Point/IPointCollection.h"
#include "Geometry/IGeometryPredicate.h"

template<typename T, int N>
class IUniquePointCollection : public IPointCollection
{
public:
   virtual std::tuple<bool, int>  tryGetClosePoint(const Point<T, N>&) const = 0;
   virtual const IGeometryPredicate& getGeometryPredicate() const = 0;
};