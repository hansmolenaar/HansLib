#pragma once

#include "BoundingBox.h"
#include "IGeometryPredicate.h"
#include "IGeometryRegion.h"
#include "DirectedEdge.h"
#include "MyAssert.h"
#include "MyException.h"

namespace Geometry
{
   enum SpherePosition { Inside, On, Outside };

   template<typename T, int N>
   class Sphere : public IGeometryRegion<T, N>
   {
   public:
      Sphere(Point<T, N> center, T radius);

      BoundingBox<T, N> getBoundingBox() const override;

      bool Contains(const Point<T, N>& point, const IGeometryPredicate<T, N>& predicate) const override;

      // First **after** start point
      // If the edge is contained in the region, then return the exit point or the end point of the edge
      // If only the first point of the edge is in the region return false
      std::tuple< bool, Point<T, N>> TryGetFirstIntersectionWithDirectedEdge(typename const Geometry::DirectedEdge<T, N>& edge) const override;

     
      SpherePosition getPosition(const Point<T, N>& point, const IGeometryPredicate<T, N>& predicate) const;
   private:
      Point<T, N> m_center;
      T m_radius;
   };

   template<typename T, int N>
   Sphere<T, N>::Sphere(Point<T, N> center, T radius) :
      m_center(std::move(center)), m_radius(radius)
   {
      Utilities::MyAssert(m_radius > 0);
   }

   template<typename T, int N>
   SpherePosition Sphere<T, N>::getPosition(const Point<T, N>& point, const IGeometryPredicate<T, N>& predicate) const
   {
      if (Contains(point, predicate)) return On;
      const T dist = PointUtils::GetNormSquared(point - m_center);
      if (dist < m_radius * m_radius) return Inside;
      return Outside;
   }

   template<typename T, int N>
   BoundingBox<T, N> Sphere<T, N>::getBoundingBox() const
   {
      std::array<T, N> lwr;
      std::array<T, N> upr;
      str::transform(m_center, lwr.begin(), [this](T cor) {return cor - m_radius; });
      str::transform(m_center, upr.begin(), [this](T cor) {return cor + m_radius; });
      return BoundingBox<T, N>::CreateFromList(std::array<std::array<T, N>, 2> {lwr, upr});
   }

   template<typename T, int N>
   bool Sphere<T, N>::Contains(const Point<T, N>& point, const IGeometryPredicate<T, N>& predicate) const
   {
      const T distToCenter2 = PointUtils::GetNormSquared(point - m_center);
      return std::abs(distToCenter2 - m_radius * m_radius) < predicate.getSmallNormSquared();
   }

   template<typename T, int N>
   std::tuple< bool, Point<T, N>> Sphere<T, N>::TryGetFirstIntersectionWithDirectedEdge(typename const Geometry::DirectedEdge<T, N>& edge) const
   {
      const auto& predicate = edge.getPredicate();
      const auto& point0 = edge.point0();
      const auto& point1 = edge.point1();
      const auto position0 = getPosition(point0, predicate);
      const auto position1 = getPosition(point1, predicate);

      if (position0 == SpherePosition::Inside)
      { 
         if (position1 == SpherePosition::Inside) return { false, {} };
         else if (position1 == SpherePosition::On) return { true, edge.point1()};
      }
#if false
      const T a = edge.lengthSquared();
      T b = 0;
      for (int n = 0; n < N; ++n)
      {
         b += 2 * (edge.point0().at(n) - edge.point1().at(n)) * (edge.point1().at(n) - m_center.at(n));
      }
      T c = PointUtils::GetNormSquared<T, N>(edge.point1() - m_center);
      c -= m_radius * m_radius;

      const T D = b * b - 4 * a * c;
      if (D < 0) return { false, {} };

      // The first root
      const T lam0 = (-b - std::sqrt(D)) / (2 * a);
      const auto getPosition = [this, &edge](T lambda)
      {
         const Point<T, N> p = edge.point0() * lambda + edge.point1() * (1 - lambda);
         if (Contains(p)) return 0;
         const T dist = PointUtils::GetNormSquared(p - m_center);
         if (dist < m_radius * m_radius) return -1;
         return 1;
      }
#endif
      throw MyException("Not yet implemented");

   }
}
