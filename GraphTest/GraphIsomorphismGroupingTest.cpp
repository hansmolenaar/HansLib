#include <gtest/gtest.h>

#include "Defines.h"
#include "GraphIsomorphismGrouping.h"
#include "Single.h"

using namespace GraphIsomorphism;
using namespace Utilities;

namespace
{
} // namespace

TEST(GraphIsomorphismGrouping, SomeInts)
{
    const std::vector<int> values{3, 2, 3, 2, 3, 1};
    const Grouping grouping(values);
    ASSERT_EQ(grouping.getGroupSizes(), (std::vector<size_t>{1, 2, 3}));
    ASSERT_EQ(grouping().at(0), (std::vector<int>{1}));
    ASSERT_EQ(grouping().at(1), (std::vector<int>{2, 2}));
    ASSERT_EQ(grouping().at(2), (std::vector<int>{3, 3, 3}));
}

TEST(GraphIsomorphismGrouping, CustomCompare)
{
    const std::vector<int> values{1, 2, 1};
    const std::vector<const int *> pointers{&values[0], &values[1], &values[2]};
    auto cmp = [](const int *p1, const int *p2) { return *p1 < *p2; };
    const Grouping grouping(pointers, cmp);
    ASSERT_EQ(grouping.getGroupSizes(), (std::vector<size_t>{2, 1}));
}
