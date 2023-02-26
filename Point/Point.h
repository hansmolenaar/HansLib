#pragma once

#include "Defines.h"
#include "MyAssert.h"

#include <algorithm>


template<typename T, int N>
class Point
{
public:
   Point() = default;
   Point(const std::array< T, N>& values) { m_values = values; }
   Point(const std::initializer_list<T>& values);
   const T& at(int n) const { return m_values.at(n); }

   T& operator [](int idx) { return m_values.at(idx); }
   T operator [](int idx) const { return m_values.at(idx); }

   using PointIterator = std::array<T, N>::const_iterator;

   PointIterator begin() const { return m_values.begin(); }
   PointIterator end() const { return m_values.end(); }

   auto operator<=>(const Point<T, N>&) const = default;

   Point<T, N> operator*(T) const;
   Point<T, N>& operator+=(const Point<T, N>&);
   Point<T, N>& operator-=(const Point<T, N>&);

   friend Point<T, N> operator+(Point<T, N> lhs, const Point<T, N>& rhs)
   {
      lhs += rhs;
      return lhs;
   }

   std::span<const T> data() const { return m_values; }

private:
   std::array<T, N>::iterator begin()  { return m_values.begin(); }
   std::array<T, N>::iterator end()  { return m_values.end(); }

   std::array<T, N> m_values;
};


template<typename T, int N>
Point<T, N>::Point(const std::initializer_list<T>& values)
{
   Utilities::MyAssert(values.size() == N);
   std::copy(values.begin(), values.end(), m_values.begin());
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
   Point<T,N> result;
   std::transform(m_values.begin(), m_values.end(), result.begin(), [factor](T val) {return factor * val; });
   return result;
}

using Point1 = Point<double, 1>;
using Point2 = Point<double, 2>;
using Point3 = Point<double, 3>;
using IntPoint1 = Point<int, 1>;
using IntPoint2 = Point<int, 2>;
using IntPoint3 = Point<int, 3>;