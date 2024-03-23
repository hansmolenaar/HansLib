#pragma once

#include "DirectedEdge.h"
#include <boost/container/static_vector.hpp>

namespace Geometry
{
   template<typename T, int N>
   using IntersectionDirectedEdges = boost::container::static_vector<DirectedEdge<T, N>, 1>;
} 