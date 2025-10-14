#pragma once

#include "DirectedEdgePoint.h"

namespace Geometry
{
   template<typename T, int N>
   struct DirectedEdgeInterval
   {
      DirectedEdgePoint<T, N> Point0;
      DirectedEdgePoint<T, N> Point1;
   };
}