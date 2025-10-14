#pragma once

#include "Point.h"
#include <array>

namespace Geometry
{
   template<typename T, size_t N>
   using Simplex = std::array<Point<T, N>, N + 1>;
}
