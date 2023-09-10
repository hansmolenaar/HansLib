#pragma once

#include "Defines.h"
#include "MyAssert.h"
#include "Rational.h"

template<typename T, int N>
using Point = std::array<T, N>;

using Point1 = Point<double, 1>;
using Point2 = Point<double, 2>;
using Point3 = Point<double, 3>;
using IntPoint1 = Point<int, 1>;
using IntPoint2 = Point<int, 2>;
using IntPoint3 = Point<int, 3>;
using RatPoint1 = Point<Rational, 1>;
using RatPoint2 = Point<Rational, 2>;
using RatPoint3 = Point<Rational, 3>;

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
std::array<T, N> operator-(std::array<T, N> result)
{
   str::transform(result, result.begin(), [](T value) {return -value; });
   return result;
}

namespace std
{
   template<>
   struct hash< IntPoint1 >
   {
      size_t operator()(const IntPoint1& point) const noexcept;
   };

   template<>
   struct hash< IntPoint2 >
   {
      size_t operator()(const IntPoint2& point) const noexcept;
   };

   template<>
   struct hash< IntPoint3 >
   {
      size_t operator()(const IntPoint3& point) const noexcept;
   };

   template<>
   struct hash< RatPoint1 >
   {
      size_t operator()(const RatPoint1& point) const noexcept;
   };

   template<>
   struct hash< RatPoint2 >
   {
      size_t operator()(const RatPoint2& point) const noexcept;
   };

   template<>
   struct hash< RatPoint3 >
   {
      size_t operator()(const RatPoint3& point) const noexcept;
   };

}