#pragma once

#include "Defines.h"
#include "Point.h"
#include "IGeometryPredicate.h"

namespace Geometry
{
   template<typename T, int N>
   class DirectedEdge
   {
   public:
      static DirectedEdge<T, N> Create(const Point<T, N>& from, const Point<T, N>& to, const IGeometryPredicate<T, N>& predicate);
      const Point<T, N>& point0() const { return m_from; }
      const Point<T, N>& point1() const { return m_to; }
      T lengthSquared() const;
      T project(const Point<T, N>& point) const;
      Point<T, N> interpolate(T lambda) const;
      bool contains(const Point<T, N>& point, const IGeometryPredicate<T, N>& predicate) const;
      bool isDegenerate(const IGeometryPredicate<T, N>& predicate) const;

   private:
      DirectedEdge(const Point<T, N>& from, const Point<T, N>& to);
      Point<T, N> m_from;
      Point<T, N> m_to;
   };

   template<typename T, int N>
   DirectedEdge<T, N>::DirectedEdge(const Point<T, N>& from, const Point<T, N>& to) :
      m_from(from), m_to(to)
   {
   }

   template<typename T, int N>
   DirectedEdge<T, N> DirectedEdge<T, N>::Create(const Point<T, N>& from, const Point<T, N>& to, const IGeometryPredicate<T, N>& predicate)
   {
      if (predicate.SamePoints(from, to))
      {
         throw MyException("DirectedEdge<T, N>::DirectedEdge same points");
      }

      return DirectedEdge<T, N>(from, to);
   }

   template<typename T, int N>
   T DirectedEdge<T, N>::lengthSquared() const
   {
      return PointUtils::GetNormSquared(m_to - m_from);
   }

   // 0 => point0
   // 1 => point1
   template<typename T, int N>
   Point<T, N> DirectedEdge<T, N>::interpolate(T lambda) const
   {
      return point0() * (1 - lambda) + point1() * lambda;
   }

   template<typename T, int N>
   T DirectedEdge<T, N>::project(const Point<T, N>& point) const
   {
      T inprod = 0;
      T norm2 = 0;
      for (int d = 0; d < N; ++d)
      {
         const T dif = (point1().at(d) - point0().at(d));
         inprod += dif * (point.at(d) - point0().at(d));
         norm2 += dif * dif;
      }

      return inprod / norm2;
   }

   template<typename T, int N>
   bool DirectedEdge<T, N>::isDegenerate(const IGeometryPredicate<T, N>& predicate) const
   {
      return predicate.SamePoints(point0(), point1());
   }

   template<typename T, int N>
   bool DirectedEdge<T, N>::contains(const Point<T, N>& point, const IGeometryPredicate<T, N>& predicate) const
   {
      // Project the point on the line
      const T lambda = project(point);


      // Projection within range of line?
      if (lambda >= 0 && lambda <= 1)
      {
         const auto projected = interpolate(lambda);
         return predicate.SamePoints(projected, point);
      }
      else if (lambda < 0)
      {
         return predicate.SamePoints(point0(), point);
      }
      else
      {
         return predicate.SamePoints(point1(), point);
      }
   }


} // namespace Geometry