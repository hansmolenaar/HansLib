#pragma once

#include <gtest/gtest.h>
#include "IGeometryRegion.h"
#include "IRegionManifoldsTest.h"

using namespace Geometry;

template<typename T, size_t N>
void IGeometryRegionTestInterface(const IGeometryRegion<T, N>& region, const IGeometryPredicate<T, N>& predicate)
{
   const BoundingBox<T, N> bb = region.getBoundingBox();
   ASSERT_TRUE(region.couldIntersectWith(bb, predicate));

   const auto diag = bb.getUpper() - bb.getLower();
   const auto pointOutside = bb.getUpper() + diag;
   ASSERT_FALSE(region.contains(pointOutside, predicate));

   const auto& manifolds = region.getManifolds();
   IRegionManifoldsTestInterface(manifolds, predicate);
}

