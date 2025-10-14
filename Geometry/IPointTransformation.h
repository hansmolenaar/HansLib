#pragma once

#include "Point.h"

template<typename T, size_t N>
class IPointTransformation
{
public:
   virtual ~IPointTransformation() = default;
   virtual Point<T, N> operator()(const Point<T, N>&) const = 0;
};
