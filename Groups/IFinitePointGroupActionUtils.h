#pragma once

#include "IFinitePointGroupAction.h"
#include "IFiniteGroupUtils.h"

namespace IFinitePointGroupActionUtils
{
   template<int N> 
   std::vector<Point<double, N>> GetOrbit(const IFinitePointGroupAction<N>&, const Point<double, N>&);
}


template<int N>
std::vector<Point<double, N>> IFinitePointGroupActionUtils::GetOrbit(const IFinitePointGroupAction<N>& groupAction, const Point<double, N>& point)
{
   std::vector<Point<double, N>> result;
   result.reserve(groupAction.getGroup().getOrder());
   str::transform(IFiniteGroupUtils::GetElements(groupAction.getGroup()), std::back_inserter(result),
      [&groupAction, &point](const auto& elm) { return groupAction(elm, point); });
   return result;
}