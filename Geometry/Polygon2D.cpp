#include "Polygon2D.h"
#include "MyException.h"
#include "Rational.h"

template<typename T>
T Polygon2D::AreaSigned(std::span<const Point<T, 2>> polygon)
{
   T result = 0;
   const auto nPoints = polygon.size();
   if (nPoints < 3)  throw MyException("Polygon2D::AreaSigned not enough points: " + std::to_string(nPoints));

   // The shoelace formula
   for (int n = 1; n < nPoints; ++n)
   {
      const auto& p1 = polygon[n - 1];
      const auto& p2 = polygon[n];
      result += (p1.at(0) * p2.at(1) - p2.at(0) * p1.at(1));
   }

   // Last edge
   const auto& p1 = polygon[nPoints - 1];
   const auto& p2 = polygon[0];

   result += (p1.at(0) * p2.at(1) - p2.at(0) * p1.at(1));
   result = result / 2;
   return result;
}

template double  Polygon2D::AreaSigned<double>(std::span<const Point<double, 2>> polygon);
template Rational Polygon2D::AreaSigned<Rational>(std::span<const Point<Rational, 2>> polygon);

double Polygon2D::TriangleAreaSigned(const Point2& p0, const Point2& p1, const Point2& p2)
{
   const std::array<Point2, 3> points{p0, p1, p2};
   return AreaSigned<double>(points);
}