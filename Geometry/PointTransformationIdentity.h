#pragma once

#include "Geometry/IPointTransformation.h"

template<typename T, int N>
class PointTransformationIdentity : public IPointTransformation<T,N>
{
   Point<T, N> operator()(const Point<T, N>&) const override;
};

template<typename T, int N>
Point<T, N> PointTransformationIdentity<T,N>::operator()(const Point<T, N>& p) const
{
   return p;
}