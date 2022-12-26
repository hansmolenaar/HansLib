#include <gtest/gtest.h>

#include "Interpolation/HierarchicalMultiIndex.h"
#include "Utilities/MyException.h"

#include "Utilities/Defines.h"

TEST(HierarchicalMultiIndex, Basics)
{
   const  HierarchicalMultiIndex mi(std::vector<size_t>{2, 3}, std::vector<size_t>{1, 8});
   ASSERT_EQ(mi.getDimension(), 2);

   std::vector<size_t> expect{ 2,3 };
   ASSERT_TRUE(str::equal(mi.getLevel(), expect));
   expect = std::vector<size_t>{1,8};
   ASSERT_TRUE(str::equal(mi.getIndex(), expect));

   ASSERT_EQ(mi.getL1NormLevel(), 5);
   ASSERT_EQ(mi.getSupNormLevel(), 3);
}
