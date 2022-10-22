#pragma once

#include "HLUtils/MessageHandler.h"
#include <array>


template<typename T, int N>
class Point
{
public:
   // Point(const std::array<const T, N>& values) { m_values = value; }
   Point(const std::initializer_list<T>& values);
   const T& at(int n) const { return m_values.at(n); }

   using PointIterator = std::array<T, N>::const_iterator;

   PointIterator begin() const { return m_values.begin(); }
   PointIterator end() const { return m_values.end(); }
private:
   std::array<T, N> m_values;
};


template<typename T, int N>
Point<T, N>::Point(const std::initializer_list<T>& values)
{
   MessageHandler::Assert(values.size() == N);
   std::copy(values.begin(), values.end(), m_values.begin());
}



using Point1 = Point<double, 1>;
using Point3 = Point<double, 3>;
using IntPoint1 = Point<int, 1>;
using IntPoint3 = Point<int, 3>;