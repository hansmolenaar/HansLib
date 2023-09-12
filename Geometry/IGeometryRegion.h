#pragma once

#include "BoundingBox.h"
#include "IGeometryPredicate.h"
#include "DirectedEdge.h"
#include "IGeometryObject.h"

namespace Geometry
{
   template<typename T, int N>
   class IGeometryRegion : public IGeometryObject<T, N>
   {
   public:
      virtual ~IGeometryRegion() noexcept = default;


      virtual bool Contains(const Point<T, N>& point, const IGeometryPredicate<T, N>& predicate) const = 0;

      // First **after** start point
      // If the edge is contained in the region, then return the exit point or the end point of the edge
      // If only the first point of the edge is in the region return false
      virtual std::tuple< bool, Point<T, N>> TryGetFirstIntersectionWithDirectedEdge(const DirectedEdge<T, N>& edge, const IGeometryPredicate<T, N>& predicate) = 0;
   };


}
