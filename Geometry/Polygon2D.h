#pragma once

#include "Point.h"
#include "IGeometryPredicate.h"
#include <span>


namespace Polygon2D
{
   template<typename T>
   T AreaSigned(std::span<const Point<T, 2>> polygon);

   template<typename T>
   bool Contains(std::span<const Point<T, 2>> polygon, const Point<T, 2>& point, const IGeometryPredicate<T, 2>& predicate);
};