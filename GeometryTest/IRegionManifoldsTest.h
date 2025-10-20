#pragma once

#include "Defines.h"
#include "IManifold0Test.h"
#include "IManifold1Test.h"
#include "IRegionManifolds.h"
#include <gtest/gtest.h>

using namespace Geometry;
using namespace Topology;

template <typename T, size_t N>
void IRegionManifoldsTestInterface(const IRegionManifolds<T, N> &manifolds, const IGeometryPredicate<T, N> &predicate)
{
    const auto allManifolds = manifolds.getAllManifolds();

    // manifolds should be unique
    auto cmp = [](const IManifoldId *a, const IManifoldId *b) { return *a < *b; };
    std::set<const IManifoldId *, decltype(cmp)> manifoldIds;
    for (const auto *m : allManifolds)
    {
        if (manifoldIds.contains(m))
        {
            throw MyException("Duplicate manifold " + m->getName());
        }
        manifoldIds.insert(m);
    }

    const auto boundary = manifolds.getBoundaryHyperManifolds();
    ASSERT_TRUE(!allManifolds.empty());
    ASSERT_TRUE(!boundary.empty());

    constexpr TopologyDimension maxDim = static_cast<TopologyDimension>(N - 1);
    ASSERT_TRUE(str::all_of(allManifolds,
                            [maxDim](const auto *manifold) { return manifold->getTopologyDimension() <= maxDim; }));
    ASSERT_TRUE(str::all_of(boundary, [maxDim](const auto *b) { return b->getTopologyDimension() == maxDim; }));
    ASSERT_TRUE(
        str::all_of(boundary, [&manifolds](const auto *b) { return manifolds.getConnectedHighers(*b).empty(); }));

    for (const auto *b : boundary)
    {
        ASSERT_TRUE(std::find(allManifolds.begin(), allManifolds.end(), b) != allManifolds.end());
    }

    const auto manifolds0 = manifolds.template getManifoldsOfType<const IManifold0<T, N> *>();
    for (const auto *manifold0 : manifolds0)
    {
        IManifold0TestInterface(*manifold0, predicate);
    }

    const auto manifolds1 = manifolds.template getManifoldsOfType<const IManifold1<T, N> *>();
    for (const auto *manifold1 : manifolds1)
    {
        IManifold1TestInterface(*manifold1, predicate);
    }
}
