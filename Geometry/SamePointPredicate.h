#pragma once

#include "Geometry/IGeometryPredicate.h"

template<typename T, int N>
struct SamePointPredicate
{
   const IGeometryPredicate<T, N>& predicate;
   Point<T, N> referencePoint;
   bool operator()(const Point<T, N>& p) const;
};

template<typename T, int N>
bool SamePointPredicate<T, N>::operator()(const Point<T, N>& p) const
{
   return predicate.SamePoints(referencePoint, p);
}