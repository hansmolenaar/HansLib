#pragma once

#include "BoundingBox.h"
#include "DirectedEdge.h"
#include "IGeometryObject.h"
#include "IGeometryPredicate.h"
#include "IRegionManifolds.h"

namespace Geometry
{
template <typename T, size_t N> class IGeometryRegion : public IGeometryObject<T, N>
{
  public:
    virtual ~IGeometryRegion() noexcept = default;

    // No means no, yes means maybe
    virtual bool couldIntersectWith(const BoundingBox<T, N> &bb, const IGeometryPredicate<T, N> &predicate) const;

    virtual bool contains(const Point<T, N> &point, const IGeometryPredicate<T, N> &predicate) const = 0;

    virtual const IRegionManifolds<T, N> &getManifolds() const = 0;

    virtual const std::string &getName() const = 0;
};

template <typename T, size_t N>
bool IGeometryRegion<T, N>::couldIntersectWith(const BoundingBox<T, N> &bb,
                                               const IGeometryPredicate<T, N> &predicate) const
{
    const BoundingBox<T, N> bbSelf = this->getBoundingBox();
    const auto overlap = BoundingBox<T, N>::TryGetOverlap(bbSelf, bb);
    return overlap.has_value();
}
} // namespace Geometry
