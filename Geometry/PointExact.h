#pragma once

#include "Point.h"
#include "IGeometryPredicate.h"
#include "MyAssert.h"

template<typename T, int N>
class PointExact : public IGeometryPredicate<T, N>
{
public:
   bool operator ()(const Point<T, N>&, const Point<T, N>&) const;
   bool SamePoints(Point<T, N> p0, Point<T, N> p1) const override;
   T getSmallLengthInDirection(int n) const override;
};

template<typename T, int N>
bool PointExact<T, N>::operator ()(const Point<T, N>& p0, const Point<T, N>& p1) const
{
   return p0 == p1;
}

template<typename T, int N>
bool PointExact<T, N>::SamePoints(Point<T, N> p0, Point<T, N> p1) const
{
   return (*this)(p0, p1);
}

template<typename T, int N>
T PointExact<T, N>::getSmallLengthInDirection(int n) const
{
   Utilities::MyAssert(n >= 0);
   Utilities::MyAssert(n < N);
   return 0;
}
