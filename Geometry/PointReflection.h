#pragma once

#include "IPointTransformation.h"

template<typename T, int N>
class PointReflection : public IPointTransformation<T, N>
{
public:
   Point<T, N> operator()(const Point<T,N>& p) const override;
};

template<typename T, int N>
Point<T, N> PointReflection<T, N>::operator()(const Point<T, N>& p) const
{
   return -p;
}