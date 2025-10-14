#pragma once

#include "DirectedEdgePoint.h"

namespace Geometry
{
   template<typename T, size_t N>
   struct DirectedEdgeInterval
   {
      DirectedEdgePoint<T, N> Point0;
      DirectedEdgePoint<T, N> Point1;
   };
}
