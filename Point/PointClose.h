#pragma once

#include "Point.h"

template<typename T, int N>
struct PointClose
{
   double eps = 1.0e-10;
   bool operator ()(const Point<T, N>&, const Point<T, N>&) const;
};

template<typename T, int N>
bool PointClose<T,N>::operator ()(const Point<T, N>& p0, const Point<T, N>& p1) const
{
   for (int n = 0; n < N; ++n)
   {
      if (std::abs(p0[n] - p1[n]) > eps) return false;
   }
   return true;
}
