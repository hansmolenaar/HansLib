#pragma once

#include "IFiniteGroup.h"
#include "Geometry/Point.h"

template<int N>
class IFinitePointGroupAction
{
   virtual ~IFinitePointGroupAction() = default;
   const IFiniteGroup& getGroup() const = 0;
   Point<double, N> operator()(int, Point<double,N>) const = 0;
};
