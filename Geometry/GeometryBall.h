#pragma once

#include "BoundingBox.h"
#include "IGeometryPredicate.h"
#include "IGeometryRegion.h"
#include "DirectedEdge.h"
#include "MyAssert.h"
#include "MyException.h"

namespace Geometry
{
   enum BallPosition { Inside, On, Outside };

   template<typename T, int N>
   class Ball : public IGeometryRegion<T, N>
   {
   public:
      Ball(Point<T, N> center, T radius);

      BoundingBox<T, N> getBoundingBox() const override;

      bool Contains(const Point<T, N>& point, const IGeometryPredicate<T, N>& predicate) const override;

      // First **after** start point
      // If the edge is contained in the region, then return the exit point or the end point of the edge
      // If only the first point of the edge is in the region return false
      std::tuple< bool, Point<T, N>> TryGetFirstIntersectionWithDirectedEdge(typename const Geometry::DirectedEdge<T, N>& edge) const override;


      BallPosition getPosition(const Point<T, N>& point, const IGeometryPredicate<T, N>& predicate) const;
      std::pair<BallPosition, BallPosition> getPositions(const DirectedEdge<T, N>& edge) const;

      Point<T, N> getCenter() const;
      T getRadius() const;
      T getRadiusSquared() const;

   private:
      Point<T, N> m_center;
      T m_radius;
      T m_radiusSquared;
   };

   template<typename T, int N>
   Ball<T, N>::Ball(Point<T, N> center, T radius) :
      m_center(std::move(center)), m_radius(radius), m_radiusSquared(radius* radius)
   {
      Utilities::MyAssert(m_radius > 0);
   }

   template<typename T, int N>
   Point<T, N> Ball<T, N>::getCenter() const
   {
      return m_center;
   }

   template<typename T, int N>
   T Ball<T, N>::getRadius() const
   {
      return m_radius;
   }

   template<typename T, int N>
   T Ball<T, N>::getRadiusSquared() const
   {
      return m_radiusSquared;
   }


   template<typename T, int N>
   BallPosition Ball<T, N>::getPosition(const Point<T, N>& point, const IGeometryPredicate<T, N>& predicate) const
   {
      const T distToCenter2 = PointUtils::GetNormSquared(point - m_center);
      if (std::abs(distToCenter2 - m_radiusSquared) <= predicate.getSmallNormSquared()) return On;
      else if (distToCenter2 < m_radiusSquared) return Inside;
      else return Outside;
   }

   template<typename T, int N>
   std::pair<BallPosition, BallPosition> Ball<T, N>::getPositions(const DirectedEdge<T, N>& edge) const
   {
      const auto& predicate = edge.getPredicate();
      return { getPosition(edge.point0(), predicate), getPosition(edge.point1(), predicate) };
   }

   template<typename T, int N>
   BoundingBox<T, N> Ball<T, N>::getBoundingBox() const
   {
      std::array<T, N> lwr;
      std::array<T, N> upr;
      str::transform(m_center, lwr.begin(), [this](T cor) {return cor - m_radius; });
      str::transform(m_center, upr.begin(), [this](T cor) {return cor + m_radius; });
      return BoundingBox<T, N>::CreateFromList(std::array<std::array<T, N>, 2> {lwr, upr});
   }

   template<typename T, int N>
   bool Ball<T, N>::Contains(const Point<T, N>& point, const IGeometryPredicate<T, N>& predicate) const
   {
      return getPosition(point, predicate) <= On;
   }

   template<typename T, int N>
   std::tuple< bool, Point<T, N>> Ball<T, N>::TryGetFirstIntersectionWithDirectedEdge(typename const Geometry::DirectedEdge<T, N>& edge) const
   {
      const auto& predicate = edge.getPredicate();
      const auto [pos0, pos1] = getPositions(edge);
      if (std::max(pos0, pos1) <= BallPosition::On)
      {
         return { true, edge.point1() };
      }
      const auto& point0 = edge.point0();
      const auto& point1 = edge.point1();
      const auto position0 = getPosition(point0, predicate);
      const auto position1 = getPosition(point1, predicate);

      const T a = edge.lengthSquared();
      T b = 0;
      for (int n = 0; n < N; ++n)
      {
         b += 2 * (edge.point1().at(n) - edge.point0().at(n)) * (edge.point0().at(n) - m_center.at(n));
      }
      T c = PointUtils::GetNormSquared<T, N>(edge.point0() - m_center);
      c -= m_radius * m_radius;

      const T D = b * b - 4 * a * c;
      if (D < 0) return { false, {} };
      const T sqrtD = static_cast<T>(std::sqrt(D));

      const T lam0 = (-b - sqrtD) / (2 * a);
      const auto ip0 = edge.interpolate(lam0);
      Utilities::MyAssert(getPosition(ip0, predicate) == BallPosition::On);

      const T lam1 = (-b + sqrtD) / (2 * a);
      const auto ip1 = edge.interpolate(lam1);
      Utilities::MyAssert(getPosition(ip1, predicate) == BallPosition::On);

      if (pos0 == BallPosition::Inside)
      {
         // Use the second root
         Utilities::MyAssert(pos1 == BallPosition::Outside);
         Utilities::MyAssert(!predicate.SamePoints(point0, ip1));
         return { true, ip1 };
      }

      if (pos0 == BallPosition::On)
      {
         Utilities::MyAssert(pos1 == BallPosition::Outside);

         // TODO better test if point is on edge
         return { lam1 <= 1 && !predicate.SamePoints(ip1, point0), ip1 };
      }

      Utilities::MyAssert(pos0 == BallPosition::Outside);
      if (predicate.SamePoints(point1, ip0)) return { true, point1 };

      // TODO better check if edge contains point
      return { lam0 <= 1, ip0 };
   }

} // namespace Geometry