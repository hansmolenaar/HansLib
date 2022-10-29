#pragma once

#include "Utilities/MessageHandler.h"
#include <array>
#include <ranges>
#include <algorithm>


template<typename T, int N>
class Point
{
public:
   Point() = default;
   Point(const std::array< T, N>& values) { m_values = values; }
   Point(const std::initializer_list<T>& values);
   const T& at(int n) const { return m_values.at(n); }

   T& operator [](int idx) { return m_values[idx]; }
   T operator [](int idx) const { return m_values[idx]; }

   using PointIterator = std::array<T, N>::const_iterator;

   PointIterator begin() const { return m_values.begin(); }
   PointIterator end() const { return m_values.end(); }

   auto operator<=>(const Point<T, N>&) const = default;

   Point<T, N>& operator+=(const Point<T, N>&);

   friend Point<T, N> operator+(Point<T, N> lhs, const Point<T, N>& rhs)
   {
      lhs += rhs;
      return lhs;
   }

private:
   std::array<T, N> m_values;
};


template<typename T, int N>
Point<T, N>::Point(const std::initializer_list<T>& values)
{
   MessageHandler::Assert(values.size() == N);
   std::copy(values.begin(), values.end(), m_values.begin());
}


template<typename T, int N>
Point<T, N>& Point<T, N>::operator+=(const Point<T, N>& rhs)
{
   std::transform(m_values.begin(), m_values.end(), rhs.m_values.begin(), m_values.begin(), [](T v0, T v1) {return v0 + v1; });
   return *this;
}

using Point1 = Point<double, 1>;
using Point2 = Point<double, 2>;
using Point3 = Point<double, 3>;
using IntPoint1 = Point<int, 1>;
using IntPoint2 = Point<int, 2>;
using IntPoint3 = Point<int, 3>;