#pragma once

#include "IPointTransformation.h"

template<typename T, size_t N>
class PointTransformationIdentity : public IPointTransformation<T,N>
{
   Point<T, N> operator()(const Point<T, N>&) const override;
};

template<typename T, size_t N>
Point<T, N> PointTransformationIdentity<T,N>::operator()(const Point<T, N>& p) const
{
   return p;
}
