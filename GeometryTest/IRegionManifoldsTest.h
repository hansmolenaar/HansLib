#pragma once

#include "Defines.h"
#include "IRegionManifolds.h"
#include <gtest/gtest.h>

using namespace Geometry;
using namespace Topology;

template<typename T, int N>
void IRegionManifoldsTestInterface(const IRegionManifolds<T, N>& manifolds, const IGeometryPredicate<T, N>& predicate)
{
   const auto allManifolds = manifolds.getAllManifolds();
   const auto boundary = manifolds.getBoundaryHyperManifolds();
   ASSERT_TRUE(!allManifolds.empty());
   ASSERT_TRUE(!boundary.empty());

   constexpr TopologyDimension maxDim = static_cast<TopologyDimension>(N - 1);
   ASSERT_TRUE(str::all_of(allManifolds, [maxDim](const auto* manifold) {return manifold->getTopologyDimension() <= maxDim; }));
   ASSERT_TRUE(str::all_of(boundary, [maxDim](const auto* b) {return b->getTopologyDimension() == maxDim; }));
   ASSERT_TRUE(str::all_of(boundary, [&manifolds](const auto* b) {return manifolds.getConnectedHighers(*b).empty(); }));

   for (const auto* b : boundary)
   {
      ASSERT_TRUE(std::find(allManifolds.begin(), allManifolds.end(), b) != allManifolds.end());
   }
}

