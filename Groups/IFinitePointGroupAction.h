#pragma once

#include "IFiniteGroup.h"
#include "Point.h"

template<int N>
class IFinitePointGroupAction
{
public:
   virtual ~IFinitePointGroupAction() = default;
   virtual const IFiniteGroup& getGroup() const = 0;
   virtual  Point<double, N> operator()(int, const Point<double, N>&) const = 0;
};
