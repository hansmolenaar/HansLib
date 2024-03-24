#pragma once

#include "DirectedEdge.h"
#include <boost/container/static_vector.hpp>

namespace Geometry
{
   template<typename T, int N>
   using DirectedEdgeIntersections = boost::container::static_vector< std::variant< Point<T, N>, DirectedEdge<T, N> >, 4>;
}