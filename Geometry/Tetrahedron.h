#pragma once

#include "IPointCollection.h"
#include "Point.h"

namespace Tetrahedron
{
   double getSignedVolume(const Point3& p0, const Point3& p1, const Point3& p2, const Point3& p3);
}
