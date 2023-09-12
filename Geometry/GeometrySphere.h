#pragma once

#include "BoundingBox.h"
#include "IGeometryPredicate.h"
#include "IGeometryRegion.h"
#include "DirectedEdge.h"
#include "MyAssert.h"

namespace Geometry
{
   template<typename T, int N>
   class Sphere : public IGeometryRegion<T, N>
   {
   public:
      Sphere(Point<T, N> center, T radius);

      BoundingBox<T, N> getBoundingBox() const override;

      bool CouldIntersectWith(const BoundingBox<T, N>& bb, const IGeometryPredicate<T, N>& predicate) const override;

      bool Contains(const Point<T, N>& point, const IGeometryPredicate<T, N>& predicate) const override;

      // First **after** start point
      // If the edge is contained in the region, then return the exit point or the end point of the edge
      // If only the first point of the edge is in the region return false
      std::tuple< bool, Point<T, N>> TryGetFirstIntersectionWithDirectedEdge(typename const Geometry::DirectedEdge<T, N>& edge, const IGeometryPredicate<T, N>& predicate) override;

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
   bool Sphere<T, N>::CouldIntersectWith(const BoundingBox<T, N>& bb, const IGeometryPredicate<T, N>& predicate) const
   {
      const auto [succes, overlap] = BoundingBox<T, N>::TryGetOverlap(bb, getBoundingBox());
      return succes;
   }

   template<typename T, int N>
   std::tuple< bool, Point<T, N>> Sphere<T, N>::TryGetFirstIntersectionWithDirectedEdge(typename const Geometry::DirectedEdge<T, N>& edge, const IGeometryPredicate<T, N>& predicate)
   {
      throw MyException("Sphere<T, N>::TryGetFirstIntersectionWithDirectedEdge not yet implemented");
   }
}
