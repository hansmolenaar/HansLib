#include <gtest/gtest.h>

#include "RenumberContiguous.h"

using namespace Utilities;

TEST(RenumberContiguousTest, Basic)
{
    const std::vector<int> values{1, -3};
    const RenumberContiguous<int, size_t> rc(values.begin(), values.end());
    ASSERT_EQ(rc.toContiguous(-3), 1);
    ASSERT_EQ(rc.at(0), 1);
}

TEST(RenumberContiguousTest, Duplicate)
{
    std::vector<int> values{};
    RenumberContiguous<size_t, int> rc(values.begin(), values.end());
    values = {1, 42, -3, 42};
    ASSERT_MYEXCEPTION_MESSAGE((rc = RenumberContiguous<size_t, int>(values.begin(), values.end())),
                               "RenumberContiguous::RenumberContiguous duplicate entry 42");
}

TEST(RenumberContiguousTest, UnknwonValue)
{
    std::vector<int> values{};
    RenumberContiguous<size_t, int> rc(values.begin(), values.end());
    ASSERT_FALSE((rc.toContiguous(42)));
}
