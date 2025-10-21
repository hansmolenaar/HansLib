#pragma once

#include "IGeometryPredicate.h"

template <typename T, size_t N> struct SamePointPredicate
{
    const IGeometryPredicate<T, N> &predicate;
    Point<T, N> referencePoint;
    bool operator()(const Point<T, N> &p) const;
};

template <typename T, size_t N> bool SamePointPredicate<T, N>::operator()(const Point<T, N> &p) const
{
    return predicate.samePoints(referencePoint, p);
}
