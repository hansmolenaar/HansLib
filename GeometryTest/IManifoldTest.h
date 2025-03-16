#pragma once

#include "IManifold.h"
#include <gtest/gtest.h>

#include <algorithm> 
#include <cstdlib>
#include <random>

using namespace Geometry;


template<typename T, int N>
void IManifoldTestInterface(const IManifold<T, N>& manifold, const IGeometryPredicate<T, N>& predicate)
{
   ASSERT_FALSE(manifold.getName().empty());
   ASSERT_TRUE(manifold == manifold);
   const auto bb = manifold.getBoundingBox();
   const auto pCenter = bb.getCenter();

   for (auto p : { bb.getLower(), bb.getUpper() })
   {
      auto del = p - pCenter;
      if (predicate.samePoints(p, pCenter))
      {
         str::generate(del, []() {return T(1); });
      }
      const auto point = pCenter + T(2) * del;
      ASSERT_FALSE(manifold.contains(point, predicate));
   }
}

