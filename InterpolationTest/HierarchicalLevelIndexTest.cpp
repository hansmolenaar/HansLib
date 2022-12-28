#include <gtest/gtest.h>

#include "Interpolation/HierarchicalLevelIndex.h"
#include "Utilities/MyException.h"


TEST(HierarchicalLevelIndex, Basis)
{
   const  HierarchicalLevelIndex li(2, 1);
   ASSERT_EQ(li.getLevel(), 2);
   ASSERT_EQ(li.getIndex(), 1);
}

TEST(HierarchicalLevelIndex, CheckLevelIndex)
{
   ASSERT_THROW(HierarchicalLevelIndex(0, 2), MyException);
   ASSERT_NO_THROW(HierarchicalLevelIndex(1, 2));
   ASSERT_THROW(HierarchicalLevelIndex(1, 3), MyException);
   ASSERT_NO_THROW(HierarchicalLevelIndex(2, 4));
   ASSERT_THROW(HierarchicalLevelIndex(2, 5), MyException);
   ASSERT_NO_THROW(HierarchicalLevelIndex(3, 8));
   ASSERT_THROW(HierarchicalLevelIndex(3, 9), MyException);
}

TEST(HierarchicalLevelIndex, SpaceShip)
{
   const  HierarchicalLevelIndex li0(2, 0);
   const  HierarchicalLevelIndex li1(2, 1);
   ASSERT_TRUE(li0 != li1);
   ASSERT_TRUE(li0 < li1);
}
