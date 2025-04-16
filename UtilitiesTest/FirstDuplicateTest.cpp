#include <gtest/gtest.h>

#include "FirstDuplicate.h"
#include <vector>

using namespace Utilities;

TEST(FirstDuplicateTest, FirstDuplicate)
{
   std::vector<int> values;
   ASSERT_FALSE(firstDuplicate(values));
   values.push_back(1);
   ASSERT_FALSE(firstDuplicate(values));
   values.push_back(2);
   ASSERT_FALSE(firstDuplicate(values));

   values.push_back(1);
   ASSERT_EQ(*firstDuplicate(values), 1);
}