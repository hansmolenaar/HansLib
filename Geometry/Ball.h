#pragma once

#include "BoundingBox.h"
#include "IGeometryPredicate.h"
#include "IGeometryObject.h"
#include "DirectedEdge.h"
#include "MyAssert.h"
#include "MyException.h"

namespace Geometry
{
   enum BallPosition { Inside, On, Outside };

   template<typename T, int N>
   class Ball : public IGeometryObject<T, N>
   {
   public:
      Ball(Point<T, N> center, T radius);

      BoundingBox<T, N> getBoundingBox() const override;

      bool Contains(const Point<T, N>& point, const IGeometryPredicate<T, N>& predicate) const;

      bool CouldIntersectWith(typename const BoundingBox<T, N>& bb, const IGeometryPredicate<T, N>& predicate) const;

      // First **after** start point
      // If the edge is contained in the region, then return the exit point or the end point of the edge
      // If only the first point of the edge is in the region return false
      std::optional<Point<T, N>> TryGetFirstIntersectionWithDirectedEdge(typename const Geometry::DirectedEdge<T, N>& edge, const IGeometryPredicate<T, N>& predicate) const;


      BallPosition getPosition(const Point<T, N>& point, const IGeometryPredicate<T, N>& predicate) const;
      std::pair<BallPosition, BallPosition> getPositions(const DirectedEdge<T, N>& edge, const IGeometryPredicate<T, N>& predicate) const;

      Point<T, N> getCenter() const;
      T getRadius() const;
      T getRadiusSquared() const;

   private:
      Point<T, N> m_center;
      T m_radius;
      T m_radiusSquared;
   };


} // namespace Geometry
