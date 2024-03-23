#pragma once

#include "Point.h"
#include <boost/container/static_vector.hpp>

namespace Geometry
{
   template<typename T, int N>
   using DirectedEdgeIntersections = boost::container::static_vector<Point<T, N>, 4>;
} 