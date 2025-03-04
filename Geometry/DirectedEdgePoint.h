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
   private:
      T m_scalar = std::numeric_limits<T>::max();
      Point<T, N> m_edgePoint;
      DirectedEdgePointType m_pointType = DirectedEdgePointType::Inside;
   };

   using DirectedEdgePoint2 = DirectedEdgePoint<double, 2>;
}