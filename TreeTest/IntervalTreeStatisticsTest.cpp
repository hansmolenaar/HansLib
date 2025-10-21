#include <gtest/gtest.h>

#include "IntervalTreeRefinePredicate.h"
#include "IntervalTreeStatistics.h"

using namespace IntervalTree;

TEST(IntervalTreeStatistics, Root)
{
    IndexTree<3> tree;

    const auto &statistics = GetStatistics<3>(tree);
    const Statistics expect{1, {1}};
    ASSERT_EQ(statistics, expect);
}

TEST(IntervalTreeStatistics, Level1)
{
    IndexTree<3> tree;
    RefineToMaxLevel<3> doRefine{2};
    tree.refineLeaves(doRefine);

    const auto &statistics = GetStatistics<3>(tree);
    std::ostringstream os;
    os << statistics;
    ASSERT_EQ(os.str(), "9, {0, 8}");
}
