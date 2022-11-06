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
   for (const auto& elm : IFiniteGroupUtils::GetElements(groupAction.getGroup()))
   {
      result.emplace_back(groupAction(elm, point));
   }
   return result;
}