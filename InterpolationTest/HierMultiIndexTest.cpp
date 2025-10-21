#include <gtest/gtest.h>

#include "Defines.h"
#include "Functors.h"
#include "HierMultiIndex.h"
#include "MyException.h"

TEST(HierMultiIndex, Basics)
{
    const HierMultiIndex mi(std::vector<HierLevelIndex>{HierLevelIndex{2, 1}, HierLevelIndex{3, 7}});
    ASSERT_EQ(mi.getDimension(), 2);

    std::vector<size_t> expect{2, 3};
    std::vector<size_t> test(2);
    str::transform(mi.get(), test.begin(), [](const HierLevelIndex &li) { return li.getLevel(); });
    ASSERT_TRUE(str::equal(test, expect));

    expect = std::vector<size_t>{1, 7};
    str::transform(mi.get(), test.begin(), [](const HierLevelIndex &li) { return li.getIndex(); });
    ASSERT_TRUE(str::equal(test, expect));

    ASSERT_EQ(mi.getL1NormLevel(), 5);
    ASSERT_EQ(mi.getSupNormLevel(), 3);
}

TEST(HierMultiIndex, Simple)
{
    const HierMultiIndex mi(HierLevelIndex{2, 1});
    ASSERT_EQ(mi.getDimension(), 1);

    ASSERT_EQ(mi.getL1NormLevel(), 2);
    ASSERT_EQ(mi.getSupNormLevel(), 2);
}

TEST(HierMultiIndex, SpaceShip)
{
    const HierMultiIndex mi(std::vector<HierLevelIndex>{{5, 5}, {7, 7}});
    auto other = HierMultiIndex(std::vector<HierLevelIndex>{{5, 7}, {4, 7}});
    ASSERT_TRUE(mi > other);
    ASSERT_TRUE(other < mi);
    ASSERT_TRUE(mi != other);
    ASSERT_TRUE(mi == mi);

    other = HierMultiIndex(std::vector<HierLevelIndex>{{5, 5}, {7, 5}});
    ASSERT_TRUE(mi > other);
    ASSERT_TRUE(other < mi);

    other = HierMultiIndex(std::vector<HierLevelIndex>{{5, 5}});
    ASSERT_TRUE(mi > other);
    ASSERT_TRUE(other < mi);
}

TEST(HierMultiIndex, ToDoubles)
{
    const HierMultiIndex mi(std::vector<HierLevelIndex>{{2, 3}, {3, 1}});
    const auto doubles = mi.toDoubles();
    const std::vector<double> expect{0.75, 0.125};
    ASSERT_TRUE(str::equal(doubles, expect, Functors::AreClose{}));
}

TEST(HierMultiIndex, RefineInDir3)
{
    const HierMultiIndex mi(std::vector<HierLevelIndex>{{2, 3}, {3, 1}, {4, 7}});

    auto refinements = mi.refine(1);
    str::sort(refinements);
    ASSERT_EQ(refinements.at(0), HierMultiIndex(std::vector<HierLevelIndex>{{2, 3}, {4, 1}, {4, 7}}));
    ASSERT_EQ(refinements.at(1), HierMultiIndex(std::vector<HierLevelIndex>{{2, 3}, {4, 3}, {4, 7}}));
}

TEST(HierMultiIndex, ToString)
{
    const HierMultiIndex mi(std::vector<HierLevelIndex>{{2, 3}, {3, 1}, {4, 7}});
    std::ostringstream os;
    os << mi;
    ASSERT_EQ(os.str(), "{2, 3} {3, 1} {4, 7}");
}