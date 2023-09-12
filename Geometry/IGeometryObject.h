#pragma once

#include "BoundingBox.h"
#include "IGeometryPredicate.h"

namespace Geometry
{
   template<typename T, int N>
   class IGeometryObject
   {
   public:
      virtual ~IGeometryObject() noexcept = default;

      // Could be infinite
      virtual BoundingBox<T, N> getBoundingBox() const = 0;

      // No means no, yes means maybe
      virtual bool CouldIntersectWith(const BoundingBox<T, N>& bb, const IGeometryPredicate<T, N>& predicate) const = 0;
   };

}
