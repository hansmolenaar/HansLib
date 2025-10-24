#include <gtest/gtest.h>

#include "UniqueSorted.h"
#include <vector>

using namespace Utilities;

TEST(UniqueSortedTest, Test)
{
    std::vector<int> values;

   UniqueSorted(values);
   ASSERT_TRUE(values.empty());

   values.push_back(2);
   UniqueSorted(values);
   ASSERT_TRUE(str::equal(values, std::vector<int>{2}));

   values.push_back(2);
   UniqueSorted(values);
   ASSERT_TRUE(str::equal(values, std::vector<int>{2}));

   values.push_back(1);
   UniqueSorted(values);
   ASSERT_TRUE(str::equal(values, std::vector<int>{1,2}));

   values.push_back(1);
   UniqueSorted(values);
   ASSERT_TRUE(str::equal(values, std::vector<int>{1,2}));

   values.push_back(1);
   values.push_back(2);
   UniqueSorted(values);
   ASSERT_TRUE(str::equal(values, std::vector<int>{1,2}));
}
