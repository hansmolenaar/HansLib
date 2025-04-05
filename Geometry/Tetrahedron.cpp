#include "Tetrahedron.h"
#include "TetrahedronNodesOriented.h"
#include "Triangle.h"
#include "UnitVector.h"

using namespace Topology;

double Tetrahedron::getSignedVolume(const Point3& p0, const Point3& p1, const Point3& p2, const Point3& p3)
{
   const auto d1 = p1 - p0;
   const auto d2 = p2 - p0;
   const auto d3 = p3 - p0;
   return PointUtils::innerProduct(d1 * d2, d3) / 6;
}

std::array<Point3, Topology::NumNodesOnTetrehadron> Tetrahedron::getRegularTetrahedron()
{
   return { Point3{1,1,1}, Point3{1,-1, -1}, Point3{-1, -1, 1}, Point3{-1, 1, -1} };
}

std::array<Point3, Topology::NumNodesOnTetrehadron> Tetrahedron::getTriRectangularTetrahedron(double a, double b, double c)
{
   return { Point3{0,0,0}, Point3{a,0,0}, Point3{0,b,0}, Point3{0,0,c} };
}

double Tetrahedron::getSmallestDihedralAngle(const Point3& p0, const Point3& p1, const Point3& p2, const Point3& p3)
{
   return getSmallestDihedralAngle(std::array<Point3, NumNodesOnTetrehadron>{p0, p1, p2, p3});
}

double Tetrahedron::getSmallestDihedralAngle(const std::array<Point3, Topology::NumNodesOnTetrehadron>& vert)
{
   std::array<std::optional<UnitVector3>, NumFacesOnTetrehadron> normal;
   str::transform(TetrahedronNodesOriented{ 0,1,2,3 }.getFaces(), normal.begin(),
      [&vert](const auto f) {return UnitVector<double, 3>::Create(Triangle::getAreaDirected(vert[f[0]], vert[f[1]], vert[f[2]])); });
   double maxInprod = std::numeric_limits<double>::lowest();
   for (size_t f0 = 0; f0 < NumNodesOnTetrehadron - 1; ++f0)
   {
      for (size_t f1 = f0 + 1; f1 < NumNodesOnTetrehadron; ++f1)
      {
         maxInprod = std::max(maxInprod, std::abs((*normal[f0]) * (*normal[f1])));
      }
   }
   return std::acos(maxInprod);
}