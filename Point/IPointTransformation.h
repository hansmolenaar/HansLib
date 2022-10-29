#pragma once

template<typename T, int N>
class IPointTransformation
{
public:
   virtual ~IPointTransformation() = default;
   virtual Point<T, N> operator()(const Point<T, N>&) = 0;
};
