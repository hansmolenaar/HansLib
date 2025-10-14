#pragma once

#include "IGeometryPredicate.h"
#include "MyAssert.h"
#include "Point.h"

template<typename T, size_t N>
class PointExact : public IGeometryPredicate<T, N>
{
public:
   bool operator ()(const Point<T, N>&, const Point<T, N>&) const;
   bool samePoints(Point<T, N> p0, Point<T, N> p1) const override;
   T getSmallLengthInDirection(int n) const override;
   T getSmallNormSquared() const override;
};

template<typename T, size_t N>
bool PointExact<T, N>::operator ()(const Point<T, N>& p0, const Point<T, N>& p1) const
{
   return p0 == p1;
}

template<typename T, size_t N>
bool PointExact<T, N>::samePoints(Point<T, N> p0, Point<T, N> p1) const
{
   return (*this)(p0, p1);
}

template<typename T, size_t N>
T PointExact<T, N>::getSmallLengthInDirection(int n) const
{
   Utilities::MyAssert(n >= 0);
   Utilities::MyAssert(n < N);
   return 0;
}

template<typename T, size_t N>
T PointExact<T, N>::getSmallNormSquared() const
{
   return 0;
}
