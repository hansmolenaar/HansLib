#include <gtest/gtest.h>

#include "Contains.h"

using namespace Utilities;

TEST(ContainsTest, Basic)
{
   const std::vector<int> values{1, 3};
   ASSERT_TRUE(Contains(values.begin(), values.end(), 1));
   ASSERT_FALSE(Contains(values.begin(), values.end(), 2));

   ASSERT_TRUE(Contains(values, 1));
   ASSERT_FALSE(Contains(values, 2));
}

TEST(ContainsTest, Function)
{
   const std::vector<int> values{1, 3};
   const IsContained<int> functor{values};
   const IsNotContained<int> functorInv{values};
   ASSERT_TRUE(functor(1));
   ASSERT_FALSE(functorInv(1));
   ASSERT_FALSE(functor(2));
   ASSERT_TRUE(functorInv(2));
}
