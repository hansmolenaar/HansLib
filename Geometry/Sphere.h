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

   template<typename T, int N>
   Sphere<T, N>::Sphere(Point<T, N> center, T radius) :
      m_ball(std::move(center), radius)
   {
   }

   template<typename T, int N>
   BoundingBox<T, N> Sphere<T, N>::getBoundingBox() const
   {
      return m_ball.getBoundingBox();
   }

   template<typename T, int N>
   bool Sphere<T, N>::Contains(const Point<T, N>& point, const IGeometryPredicate<T, N>& predicate) const
   {
      return m_ball.getPosition(point, predicate) == BallPosition::On;
   }

   template<typename T, int N>
   std::optional<Point<T, N>> Sphere<T, N>::TryGetFirstIntersectionWithDirectedEdge(typename const Geometry::DirectedEdge<T, N>& edge, const IGeometryPredicate<T, N>& predicate) const
   {
      const auto& point0 = edge.point0();
      const auto& point1 = edge.point1();

      const auto [pos0, pos1] = m_ball.getPositions(edge, predicate);

      if (pos0 == BallPosition::Inside && pos1 == BallPosition::Inside) return {};
      if (pos0 == BallPosition::Inside && pos1 == BallPosition::On) return  point1;
      if (pos0 == BallPosition::On && pos1 == BallPosition::Inside) return  {};
      if (pos0 == BallPosition::On && pos1 == BallPosition::On) return  point1;

      const auto ip = m_ball.TryGetFirstIntersectionWithDirectedEdge(edge, predicate);

      if (pos0 == BallPosition::Inside)
      {
         Utilities::MyAssert(pos1 == BallPosition::Outside);
         return *ip;
      }

      if (pos0 == BallPosition::On)
      {
         Utilities::MyAssert(pos1 == BallPosition::Outside);
         return ip;
      }

      Utilities::MyAssert(pos0 == BallPosition::Outside);
      return  ip;
   }


   template<typename T, int N>
   bool Sphere<T, N>::CouldIntersectWith(typename const BoundingBox<T, N>& bb, const IGeometryPredicate<T, N>& predicate) const
   {
      if (!BoundingBox<T, N>::TryGetOverlap(getBoundingBox(), bb)) return false;

      // If the whole bb is inside the sphere, there is no intersection
      if (m_ball.getPosition(bb.getLower(), predicate) != BallPosition::Inside) return true;
      if (m_ball.getPosition(bb.getUpper(), predicate) != BallPosition::Inside) return true;
      return false;
   }

   template<typename T, int N>
   Point<T, N> Sphere<T, N>::getCenter() const
   {
      return m_ball.getCenter();
   }

} // namespace Geometry
