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
}