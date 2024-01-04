#pragma once

#include "Point.h"

namespace Triangle
{
   double AreaSigned(const Point2& p0, const Point2& p1, const Point2& p2);
   double SmallestAngle(const Point2& p0, const Point2& p1, const Point2& p2);
}
