#pragma once

#include "IGeometryPredicate.h"

namespace Geometry
{
   // forward declaration
   template<typename T, int N>
   class DirectedEdge;

   enum class DirectedEdgePointType { Point0, Inside, Point1 };

   template<typename T, int N>
   class DirectedEdgePoint
   {
   public:
      DirectedEdgePoint<T, N>();

      DirectedEdgePoint<T, N>(const Point<T, N>& point, const DirectedEdge<T, N>& edge, const IGeometryPredicate<T, N>& predicate);
      DirectedEdgePointType getPointType() const;
      const Point<T, N>& getPoint() const;
      T getScalar() const;
   private:
      T m_scalar = std::numeric_limits<T>::max();
      Point<T, N> m_edgePoint;
      DirectedEdgePointType m_pointType = DirectedEdgePointType::Inside;
   };

   using DirectedEdgePoint2 = DirectedEdgePoint<double, 2>;

   template<typename T, int N>
   struct DirectedEdgePointEquals
   {
      const IGeometryPredicate<T, N>& predicate;

      bool operator()(DirectedEdgePoint<T, N> const& lhs, DirectedEdgePoint<T, N>  const& rhs) const
      {
         if (lhs.getPointType() != rhs.getPointType())
         {
            return false;
         }
         if (lhs.getPointType() != DirectedEdgePointType::Inside)
         {
            return true;
         }
         // both are inside
         return predicate.SamePoints(lhs.getPoint(), rhs.getPoint());
      }
   };

   template<typename T, int N>
   struct DirectedEdgePointLess
   {
      const IGeometryPredicate<T, N>& predicate;

      bool operator()(DirectedEdgePoint<T, N> const& lhs, DirectedEdgePoint<T, N>  const& rhs) const
      {
         if (lhs.getPointType() != rhs.getPointType())
         {
            return lhs.getPointType() < rhs.getPointType();
         }
         if (lhs.getPointType() != DirectedEdgePointType::Inside)
         {
            return false;
         }
         // both are inside
         if (predicate.SamePoints(lhs.getPoint(), rhs.getPoint()))
         {
            return false;
         }

         return lhs.getScalar() < rhs.getScalar();
      }
   };
}