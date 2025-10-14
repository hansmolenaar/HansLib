#pragma once

#include "IGeometryPredicate.h"
#include "IPointCollection.h"

#include <optional>

template<typename T, size_t N>
class IUniquePointCollection : public IPointCollection<T, N>
{
public:
   virtual std::optional<PointIndex>  tryGetClosePoint(const Point<T, N>&) const = 0;
   virtual const IGeometryPredicate<T, N>& getGeometryPredicate() const = 0;
};

using IUniquePointCollection2 = IUniquePointCollection<double, GeomDim2>;
using IUniquePointCollection3 = IUniquePointCollection<double, GeomDim3>;
