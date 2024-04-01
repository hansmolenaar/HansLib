#pragma once

#include "DirectedEdge.h"
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


   template<typename T, int N>
   struct DirectedEdgeInterval
   {
      DirectedEdgePoint<T, N> Point0;
      DirectedEdgePoint<T, N> Point1;
   };

   template<typename T, int N>
   using DirectedEdgeIntersections = boost::container::static_vector< std::variant< Point<T, N>, DirectedEdgeInterval<T, N> >, 4>;
}