#pragma once

#include <boost/container/static_vector.hpp>

namespace Geometry
{
   enum class DirectedEdgePointType {Point0, Inside, Point1};

   template<typename T, int N>
   struct DirectedEdgePoint
   {
      Point<T, N> EdgePoint;
      DirectedEdgePointType PointType;
   };

   using DirectedEdgePoint2 = DirectedEdgePoint<double, 2>;

   template<typename T, int N>
   struct DirectedEdgeInterval
   {
      DirectedEdgePoint<T, N> Point0;
      DirectedEdgePoint<T, N> Point1;
   };

   template<typename T, int N>
   using DirectedEdgeIntersections = boost::container::static_vector< std::variant< DirectedEdgePoint<T, N>, DirectedEdgeInterval<T, N> >, 4>;
}