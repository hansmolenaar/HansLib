#pragma once

#include "Defines.h"
#include "IGeometryPredicate.h"
#include "UnitVector.h"

namespace Geometry
{
   template<typename T, int N>
   class Line
   {

   public:
      Line(Point<T, N> refPoint, UnitVector<T, N> direction);
      Line(const Point<T, N>& p0, const Point<T, N>& p1);

      Point<T, N> project(const Point<T, N>& point) const;
      bool contains(const Point<T, N>& point, const IGeometryPredicate<T, N>& predicate) const;

   private:

      Point<T, N> m_referencePoint;
      UnitVector<T, N> m_direction;
   };

   template<typename T, int N>
   Line<T, N>::Line(Point<T, N> refPoint, UnitVector<T, N> direction) :
      m_referencePoint(std::move(refPoint)), m_direction(std::move(direction))
   {
   }

   template<typename T, int N>
   Line<T, N>::Line(const Point<T, N>& p0, const Point<T, N>& p1) :
      Line<T, N>::Line(p0, UnitVector<T, N>::Create(p0, p1).value())
   {
   }

   template<typename T, int N>
   Point<T, N> Line<T, N>::project(const Point<T, N>& point) const
   {
      const auto dif = point - m_referencePoint;
      const T inprod = m_direction.innerProduct(dif);
      return  m_referencePoint + inprod * m_direction;
   }

   template<typename T, int N>
   bool Line<T, N>::contains(const Point<T, N>& point, const IGeometryPredicate<T, N>& predicate) const
   {
      // Project the point on the line
      const auto projected = project(point);
      return predicate.SamePoints(projected, point);
   }

} // namespace Geometry