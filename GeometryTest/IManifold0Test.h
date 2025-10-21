#pragma once

#include "IManifold0.h"
#include "IManifoldTest.h"
#include <gtest/gtest.h>

#include <algorithm>
#include <cstdlib>
#include <random>

using namespace Geometry;

template <typename T, size_t N>
void IManifold0TestInterface(const IManifold0<T, N> &manifold, const IGeometryPredicate<T, N> &predicate)
{
    IManifoldTestInterface(manifold, predicate);
    ASSERT_EQ(manifold.getTopologyDimension(), Topology::Corner);
    const auto point = manifold.getPoint();
    for (const auto &p : point)
    {
        ASSERT_TRUE(p == p);
    }
}
