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
