#pragma once

#include "Point.h"
#include <span>


namespace Polygon2D
{
   template<typename T>
   T AreaSigned(std::span<const Point<T, 2>> polygon);

   double TriangleAreaSigned(const Point2& p0, const Point2& p1, const Point2& p2);
};