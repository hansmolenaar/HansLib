#pragma once

#include "Point.h"
#include <array>

namespace Geometry
{
   template<typename T, int N>
   using Simplex = std::array<Point<T, N>, N + 1>;
}