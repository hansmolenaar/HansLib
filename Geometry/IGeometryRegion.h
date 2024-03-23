#pragma once

#include "BoundingBox.h"
#include "IGeometryPredicate.h"
#include "DirectedEdge.h"
#include "IGeometryObject.h"

#include <optional>

namespace Geometry
{
   template<typename T, int N>
   class IGeometryRegion : public IGeometryObject<T, N>
   {
   public:
      virtual ~IGeometryRegion() noexcept = default;

      // No means no, yes means maybe
      virtual bool CouldIntersectWith(typename const BoundingBox<T, N>& bb, const IGeometryPredicate<T, N>& predicate) const;

      virtual bool Contains(const Point<T, N>& point, const IGeometryPredicate<T, N>& predicate) const = 0;

      // First **after** start point
      // If the edge is contained in the region, then return the exit point or the end point of the edge
      // If only the first point of the edge is in the region return false
      virtual std::optional<Point<T, N>> TryGetFirstIntersectionWithDirectedEdge(const DirectedEdge<T, N>& edge, const IGeometryPredicate<T, N>& predicate) const = 0;
   };

   template<typename T, int N>
   bool IGeometryRegion<T,N>::CouldIntersectWith(typename const BoundingBox<T, N>& bb, const IGeometryPredicate<T, N>& predicate) const
   {
      const BoundingBox<T,N> bbSelf = this->getBoundingBox();
      const auto overlap = BoundingBox<T, N>::TryGetOverlap(bbSelf, bb);
      return overlap.has_value();
   }
}
