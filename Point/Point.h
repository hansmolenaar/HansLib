#pragma once

#include "Defines.h"
#include "MyAssert.h"
#include "Rational.h"

template<typename T, int N>
using Point = std::array<T, N>;

using Point1 = Point<double, GeomDim1>;
using Point2 = Point<double, GeomDim2>;
using Point3 = Point<double, GeomDim3>;
using IntPoint1 = Point<int, GeomDim1>;
using IntPoint2 = Point<int, GeomDim2>;
using IntPoint3 = Point<int, GeomDim3>;
using RatPoint1 = Point<Rational, GeomDim1>;
using RatPoint2 = Point<Rational, GeomDim2>;
using RatPoint3 = Point<Rational, GeomDim3>;

template<typename T, int N>
std::array<T, N> operator+(std::array<T, N> lhs, const std::array<T, N>& rhs)
{
   std::transform(lhs.begin(), lhs.end(), rhs.begin(), lhs.begin(), std::plus<T>());
   return lhs;
}

template<typename T, int N>
std::array<T, N> operator-(std::array<T, N> lhs, const std::array<T, N>& rhs)
{
   std::transform(lhs.begin(), lhs.end(), rhs.begin(), lhs.begin(), std::minus<T>());
   return lhs;
}

template<typename T, int N>
std::array<T, N> operator*(std::array<T, N> result, T factor)
{
   str::transform(result, result.begin(), [factor](T value) {return factor * value; });
   return result;
}

template<typename T, int N>
std::array<T, N> operator*(T factor, std::array<T, N> result)
{
   return result * factor;
}

template<typename T, int N>
std::array<T, N> operator-(std::array<T, N> result)
{
   str::transform(result, result.begin(), [](T value) {return -value; });
   return result;
}

namespace PointUtils
{
   template<typename T, int N>
   T GetNormSquared(typename const Point<T, N>& point)
   {
      return std::inner_product(point.begin(), point.end(), point.begin(), T(0));
   }

   template<typename T, int N>
   T GetDistanceSquared(const Point<T, N>& point0, const Point<T, N>& point1)
   {
      const auto dif = point0 - point1;
      return GetNormSquared(dif);
   }

   template<typename T, int N>
   T Angle(const Point<T, N>& p0, const Point<T, N>& p1, const Point<T, N>& p2)
   {
      const auto d0 = p1 - p0;
      const auto d2 = p1 - p2;
      const T inprod = std::inner_product(d0.begin(), d0.end(), d2.begin(), T(0));
      const T norm0_squared = GetNormSquared(d0);
      const T norm2_squared = GetNormSquared(d2);
      const T norm02 = std::sqrt(norm0_squared * norm2_squared);
      constexpr double eps = 1.0e-12;
      if (norm02 < eps) throw MyException("Point::Angle degenerated");
      return std::acos(inprod / norm02);
   }

   template<typename T, int N>
   T innerProduct(const Point<T, N>& p0, const Point<T, N>& p1)
   {
      return std::inner_product(p0.begin(), p0.end(), p1.begin(), T(0));
   }
}