#include "Tetrahedron.h"

double Tetrahedron::getSignedVolume(const Point3& p0, const Point3& p1, const Point3& p2, const Point3& p3)
{
   const auto d1 = p1 - p0;
   const auto d2 = p2 - p0;
   const auto d3 = p3 - p0;
   return PointUtils::innerProduct(d1 * d2, d3) / 6;
}