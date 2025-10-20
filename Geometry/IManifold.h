#pragma once

#include "IGeometryObject.h"
#include "IGeometryPredicate.h"
#include "IManifoldId.h"

namespace Geometry
{
template <typename T, size_t N> class IManifold : public IGeometryObject<T, N>, public IManifoldId
{
  public:
    virtual bool contains(const Point<T, N> &point, const IGeometryPredicate<T, N> &predicate) const = 0;
};

} // namespace Geometry
