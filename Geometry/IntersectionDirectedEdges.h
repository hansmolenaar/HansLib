#pragma once

#include "Point.h"
#include <array>

namespace Geometry
{
   template<typename T, int N>
   using IntersectionDirectedEdges = boost::container::static_vector<DirectedEdge<T, N>, 1>;
}