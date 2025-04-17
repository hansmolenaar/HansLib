#include <gtest/gtest.h>

#include "RenumberContiguous.h"


using namespace Utilities;

TEST(RenumberContiguousTest, Basic)
{
   std::vector<int> values{ 1, -3 };
   const RenumberContiguous<size_t, std::vector<int>> rc(values);
   ASSERT_EQ(rc.toContiguous(-3), 1);
   ASSERT_EQ(rc[0], 1);
}

TEST(RenumberContiguousTest, Duplicate)
{
   RenumberContiguous<size_t, std::vector<int>> rc({});
   ASSERT_MYEXCEPTION_MESSAGE((rc = RenumberContiguous<size_t, std::vector<int>>(std::vector<int>{ 1, 42, -3, 42 })), "RenumberContiguous::RenumberContiguous duplicate entry 42");
}

TEST(RenumberContiguousTest, UnknwonValue)
{
   RenumberContiguous<size_t, std::vector<int>> rc({});
   ASSERT_MYEXCEPTION_MESSAGE((rc.toContiguous(42)), "RenumberContiguous<CTcontiguous, C>::toContiguous unknown value 42");
}
