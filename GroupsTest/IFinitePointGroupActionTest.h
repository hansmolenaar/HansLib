#pragma once

#include "Groups/IFinitePointGroupAction.h"
#include "Geometry//PointClose.h"
#include "Groups/IFiniteGroupUtils.h"

#include <numbers>

template <int N>
Point<double, N> GetRandomPoint()
{
   Point<double, N> result;
   if (N >= 1) result[0] = std::numbers::pi;
   if (N >= 2) result[1] = std::numbers::e;
   if (N >= 3) result[2] = std::numbers::ln10;
   if (N >= 4) result[3] = std::numbers::sqrt2;
   if (N >= 5) throw MyException("GetRandomPoint() not yet implemted: " + std::to_string(N));
   return result;
}

template<int N>
void Test_IFinitePointGroupAction(const IFinitePointGroupAction<N>& groupAction, const Point<double, N>& pointInSet)
{
   const PointClose<double, N> areClose;
   const auto& group = groupAction.getGroup();
   const auto groupElements = IFiniteGroupUtils::GetElements(group);

   // Origin must be invariant
   const Point<double, N> nul(std::array<double, N>{});
   for (auto elm : groupElements)
   {
      const auto retval = groupAction(elm, nul);
      ASSERT_TRUE(areClose(nul, retval));
   }

   // Off by one
   ASSERT_ANY_THROW(groupAction(group.getOrder(), nul));

   // Identity
   const auto id = group.getIdentity();
   ASSERT_TRUE(areClose(pointInSet, groupAction(id, pointInSet)));

   // Compatibility
   for (auto elm1 : groupElements)
   {
      for (auto elm2 : groupElements)
      {
         const auto elm3 = group(elm1, elm2);
         const auto point3 = groupAction(elm3, pointInSet);

         const auto point2 = groupAction(elm2, pointInSet);
         const auto point12 = groupAction(elm1, point2);
         ASSERT_TRUE(areClose(point12, point3));
      }
   }

}
