#include <gtest/gtest.h>

#include "StdHash.h"

#include <unordered_map>

TEST(StdHashTests, Basics)
{
   ArrayHasher<int,2> ah;
   const std::array<int, 2> arr1{1,1};
   const std::array<int, 2> arr2{2,2};
   const auto hash1 = ArrayHasher<int,2>{}(arr1);
   const auto hash2 = ah(arr2);
   ASSERT_NE(hash1, hash2);
}

TEST(StdHashTests, UseInUnorderedMap)
{
   std::unordered_map< std::array<int,2>, double, ArrayHasher<int,2>> map;
   const std::array<int,2> arr{1,2};
   map[arr] = 42.0;
}
