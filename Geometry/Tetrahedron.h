#pragma once

#include "IPointCollection.h"
#include "Point.h"
#include "TopologyDefines.h"

namespace Tetrahedron
{
double getSignedVolume(const Point3 &p0, const Point3 &p1, const Point3 &p2, const Point3 &p3);
std::array<Point3, Topology::NumCornersOnTetrahedron> getRegularTetrahedron();
std::array<Point3, Topology::NumCornersOnTetrahedron> getTriRectangularTetrahedron(double a = 1, double b = 1,
                                                                                   double c = 1);

double getSmallestDihedralAngle(const std::array<Point3, Topology::NumCornersOnTetrahedron> &vert);      // [rad]
double getSmallestDihedralAngle(const Point3 &p0, const Point3 &p1, const Point3 &p2, const Point3 &p3); // [rad]
} // namespace Tetrahedron
