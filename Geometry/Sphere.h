#pragma once

#include "Ball.h"

namespace Geometry
{

   template<typename T, int N>
   class Sphere : public IGeometryObject<T, N>
   {
   public:
      Sphere(Point<T, N> center, T radius);

      BoundingBox<T, N> getBoundingBox() const override;

      bool Contains(const Point<T, N>& point, const IGeometryPredicate<T, N>& predicate) const;

      // First **after** start point
      // If the edge is contained in the region, then return the exit point or the end point of the edge
      // If only the first point of the edge is in the region return false
      std::optional<Point<T, N>> TryGetFirstIntersectionWithDirectedEdge(typename const Geometry::DirectedEdge<T, N>& edge, const IGeometryPredicate<T, N>& predicate) const;

      bool CouldIntersectWith(typename const BoundingBox<T, N>& bb, const IGeometryPredicate<T, N>& predicate) const;

      Point<T, N> getCenter() const;
   private:
      Ball<T, N> m_ball;
   };


} // namespace Geometry
