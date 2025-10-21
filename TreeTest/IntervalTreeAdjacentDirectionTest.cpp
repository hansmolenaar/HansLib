#include <gtest/gtest.h>

#include "IntervalTreeAdjacentDirection.h"
#include <set>

using namespace IntervalTree;

TEST(IntervalTreeAdjacentDirection, Get1)
{
    const auto ad = GetAdjacentNeighbors<1>();
    const std::set<AdjacentDirection> adjDirs(ad.begin(), ad.end());
    ASSERT_EQ(adjDirs.size(), 2);
}
