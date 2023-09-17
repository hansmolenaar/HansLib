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
      const IGeometryPredicate<T, N>& getPredicate() const;

      Point<T, N> interpolate(T lambda) const;

   private:
      DirectedEdge(const Point<T, N>& from, const Point<T, N>& to, const IGeometryPredicate<T, N>& predicate);
      Point<T, N> m_from;
      Point<T, N> m_to;
      const IGeometryPredicate<T, N>& m_predicate;
   };

   template<typename T, int N>
   DirectedEdge<T, N>::DirectedEdge(const Point<T, N>& from, const Point<T, N>& to, const IGeometryPredicate<T, N>& predicate) :
      m_from(from), m_to(to), m_predicate(predicate)
   {
   }

   template<typename T, int N>
   DirectedEdge<T, N> DirectedEdge<T, N>::Create(const Point<T, N>& from, const Point<T, N>& to, const IGeometryPredicate<T, N>& predicate)
   {
      if (predicate.SamePoints(from, to))
      {
         throw MyException("DirectedEdge<T, N>::DirectedEdge same points");
      }

      return DirectedEdge<T, N>(from, to, predicate);
   }

   template<typename T, int N>
   T DirectedEdge<T, N>::lengthSquared() const
   {
      return PointUtils::GetNormSquared(m_to - m_from);
   }

   template<typename T, int N>
   const IGeometryPredicate<T, N>& DirectedEdge<T, N>::getPredicate() const
   {
      return m_predicate;
   }

   // 0 => point0
   // 1 => point1
   template<typename T, int N>
   Point<T, N> DirectedEdge<T, N>::interpolate(T lambda) const
   {
      return point0() * (1 - lambda) + point1() * lambda;
   }
}