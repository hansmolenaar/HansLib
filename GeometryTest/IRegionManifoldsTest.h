#pragma once

#include <gtest/gtest.h>
#include "IRegionManifolds.h"
#include "Defines.h"

using namespace Geometry;
using namespace Topology;

template<typename T, int N>
void IRegionManifoldsTestInterface(const IRegionManifolds<T, N>& manifolds, const IGeometryPredicate<T, N>& predicate)
{
   const auto allManifolds = manifolds.GetAllManifolds();
   const auto boundary = manifolds.GetBoundaryHyperManifolds();
   ASSERT_TRUE(!allManifolds.empty());
   ASSERT_TRUE(!boundary.empty());

   constexpr TopologyDimension maxDim = static_cast<TopologyDimension>(N - 1);
   ASSERT_TRUE(str::all_of(allManifolds, [maxDim](const auto* manifold) {return manifold->GetTopologyDimension() <= maxDim; }));
   ASSERT_TRUE(str::all_of(boundary, [maxDim](const auto* b) {return b->GetTopologyDimension() == maxDim; }));
   ASSERT_TRUE(str::all_of(boundary, [&manifolds](const auto* b) {return manifolds.GetConnectedHighers(*b).empty(); }));

   for (const auto* b : boundary)
   {
      ASSERT_TRUE(std::find(allManifolds.begin(), allManifolds.end(), b) != allManifolds.end());
   }
}

