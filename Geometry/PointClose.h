#pragma once

#include "Point.h"
#include "IGeometryPredicate.h"
#include "MyAssert.h"

template<typename T, int N>
class PointClose : public IGeometryPredicate<T, N>
{
public:
   bool operator ()(const Point<T, N>&, const Point<T, N>&) const;
   bool SamePoints(Point<T, N> p0, Point<T, N> p1) const override;
   T getSmallLengthInDirection(int n) const override;
private:
   double eps = 1.0e-10;
};

template<typename T, int N>
bool PointClose<T, N>::operator ()(const Point<T, N>& p0, const Point<T, N>& p1) const
{
   for (int n = 0; n < N; ++n)
   {
      if (std::abs(p0.data()[n] - p1.data()[n]) > eps) return false;
   }
   return true;
}


template<typename T, int N>
bool PointClose<T, N>::SamePoints(Point<T, N> p0, Point<T, N> p1) const
{
   return (*this)(p0, p1);
}

template<typename T, int N>
T PointClose<T, N>::getSmallLengthInDirection(int n) const
{
   Utilities::MyAssert(n >= 0);
   Utilities::MyAssert(n < N);
   return static_cast<T>(eps);
}