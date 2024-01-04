#pragma once

#include "Point.h"
#include <span>


namespace Polygon2D
{
   template<typename T>
   T AreaSigned(std::span<const Point<T, 2>> polygon);
};