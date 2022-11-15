#pragma once

#include "Point/Point.h"
#include "Geometry/IGeometryPredicate.h"

template<typename T, int N>
class PointClose : public IGeometryPredicate<T, N>
{
public:
   bool operator ()(const Point<T, N>&, const Point<T, N>&) const;
   bool SamePoints(Point<T, N> p0, Point<T, N> p1) const override;
private:
   double eps = 1.0e-10;
};

template<typename T, int N>
bool PointClose<T, N>::operator ()(const Point<T, N>& p0, const Point<T, N>& p1) const
{
   for (int n = 0; n < N; ++n)
   {
      if (std::abs(p0[n] - p1[n]) > eps) return false;
   }
   return true;
}


template<typename T, int N>
bool PointClose<T, N>::SamePoints(Point<T, N> p0, Point<T, N> p1) const
{
   return (*this)(p0, p1);
}
