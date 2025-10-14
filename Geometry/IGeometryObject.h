#pragma once

#include "BoundingBox.h"
#include "IGeometryPredicate.h"

namespace Geometry
{
   template<typename T, size_t N>
   class IGeometryObject
   {
   public:
      virtual ~IGeometryObject() noexcept = default;

      // Could be infinite
      virtual BoundingBox<T, N> getBoundingBox() const = 0;
   };

}
