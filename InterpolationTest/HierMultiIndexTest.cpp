#include <gtest/gtest.h>

#include "Interpolation/HierMultiIndex.h"
#include "Utilities/MyException.h"

#include "Utilities/Defines.h"

TEST(HierMultiIndex, Basics)
{
   const HierMultiIndex mi(std::vector<HierLevelIndex>{HierLevelIndex{ 2,1 }, HierLevelIndex{ 3,7 }});
   ASSERT_EQ(mi.getDimension(), 2);

   std::vector<size_t> expect{ 2,3 };
   std::vector<size_t> test(2);
   str::transform(mi.get(), test.begin(), [](const HierLevelIndex& li) {return li.getLevel(); });
   ASSERT_TRUE(str::equal(test, expect));

   expect = std::vector<size_t>{ 1,7 };
   str::transform(mi.get(), test.begin(), [](const HierLevelIndex& li) {return li.getIndex(); });
   ASSERT_TRUE(str::equal(test, expect));

   ASSERT_EQ(mi.getL1NormLevel(), 5);
   ASSERT_EQ(mi.getSupNormLevel(), 3);
}


TEST(HierMultiIndex, Simple)
{
   const HierMultiIndex mi(HierLevelIndex{ 2,1 });
   ASSERT_EQ(mi.getDimension(), 1);

   ASSERT_EQ(mi.getL1NormLevel(), 2);
   ASSERT_EQ(mi.getSupNormLevel(), 2);
}
