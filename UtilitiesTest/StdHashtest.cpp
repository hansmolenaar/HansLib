#include <gtest/gtest.h>

#include "StdHash.h"

TEST(StdHashTests, Basics)
{
#if false
   const std::array<int, 2> arr1{1,1};
   const std::array<int, 2> arr2{2,2};
   const auto hash1 = std::hash(arr1);
   const auto hash2 = std::hash(arr2);
   ASSERT_NE(hash1, hash2);
#endif
}