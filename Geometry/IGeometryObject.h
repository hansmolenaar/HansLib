#pragma once

#include "BoundBox.h"
#include "IGeometryPredicate.h"

template<typename T, int N>
class IGeometryObject
{
public:
   virtual ~IGeometryPredicate() noexcept = default;

   // Could be infinite
   virtual BoundingBox<T, N> getBoundingBox() const = 0;

   // No means no, yes means maybe
   virtual bool CouldIntersectWith(const BoundingBox<T, N>& bb, const IGeometryPredicate<t,N>& predicate) const = 0;
};
