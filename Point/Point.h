#pragma once

#include "Defines.h"
#include "MyAssert.h"
#include "Rational.h"

#include <functional>

template<typename T, int N>
class Point
{
public:
   Point() = default;
   Point(const std::array< T, N>& values) { m_values = values; }
   explicit Point(const std::initializer_list<T>& values);

   using PointIterator = std::array<T, N>::const_iterator;

   PointIterator begin() const { return m_values.begin(); }
   PointIterator end() const { return m_values.end(); }

   auto operator<=>(const Point<T, N>&) const = default;

   Point<T, N> operator*(T) const;
   Point<T, N>& operator+=(const Point<T, N>&);
   Point<T, N>& operator-=(const Point<T, N>&);
   Point<T, N> operator-() const;

   friend std::array<T, N> operator+(Point<T, N> lhs, const Point<T, N>& rhs)
   {
      lhs += rhs;
      return lhs.data();
   }

   const std::array<T, N>& data() const { return m_values; }

private:
   std::array<T, N> m_values;
};


template<typename T, int N>
Point<T, N>::Point(const std::initializer_list<T>& values)
{
   Utilities::MyAssert(values.size() == N);
   str::copy(values, m_values.begin());
}


template<typename T, int N>
Point<T, N>& Point<T, N>::operator+=(const Point<T, N>& rhs)
{
   std::transform(m_values.begin(), m_values.end(), rhs.m_values.begin(), m_values.begin(), [](T v0, T v1) {return v0 + v1; });
   return *this;
}

template<typename T, int N>
Point<T, N>& Point<T, N>::operator-=(const Point<T, N>& rhs)
{
   std::transform(m_values.begin(), m_values.end(), rhs.m_values.begin(), m_values.begin(), [](T v0, T v1) {return v0 - v1; });
   return *this;
}


template<typename T, int N>
Point<T, N> Point<T, N>::operator*(T factor) const
{
   Point<T, N> result;
   str::transform(m_values, result.m_values.begin(), [factor](T val) {return factor * val; });
   return result;
}

template<typename T, int N>
Point<T, N> Point<T, N>::operator-() const
{
   Point<T, N> result;
   str::transform(m_values, result.m_values.begin(), [](T value) {return -value; });
   return result;
}

using Point1 = Point<double, 1>;
using Point2 = Point<double, 2>;
using Point3 = Point<double, 3>;
using IntPoint1 = Point<int, 1>;
using IntPoint2 = Point<int, 2>;
using IntPoint3 = Point<int, 3>;
using RatPoint1 = Point<Rational, 1>;
using RatPoint2 = Point<Rational, 2>;
using RatPoint3 = Point<Rational, 3>;

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