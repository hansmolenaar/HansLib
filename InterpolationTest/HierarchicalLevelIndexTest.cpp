#include <gtest/gtest.h>

#include "Interpolation/HierarchicalLevelIndex.h"
#include "Utilities/MyException.h"


inline constexpr double Epsilon = 1.0e-10;

TEST(HierarchicalLevelIndex, Basis)
{
   const  HierarchicalLevelIndex li(2, 1);
   ASSERT_EQ(li.getLevel(), 2);
   ASSERT_EQ(li.getIndex(), 1);
}

TEST(HierarchicalLevelIndex, CheckLevelIndex)
{
   ASSERT_THROW(HierarchicalLevelIndex(0, 2), MyException);
   ASSERT_THROW(HierarchicalLevelIndex(1, 2), MyException);
   ASSERT_THROW(HierarchicalLevelIndex(1, 3), MyException);
   ASSERT_THROW(HierarchicalLevelIndex(2, 4), MyException);
   ASSERT_THROW(HierarchicalLevelIndex(2, 5), MyException);
   ASSERT_THROW(HierarchicalLevelIndex(3, 8), MyException);
   ASSERT_THROW(HierarchicalLevelIndex(3, 9), MyException);
}

TEST(HierarchicalLevelIndex, SpaceShip)
{
   const  HierarchicalLevelIndex li0(2, 1);
   const  HierarchicalLevelIndex li1(2, 3);
   ASSERT_TRUE(li0 != li1);
   ASSERT_TRUE(li0 < li1);
}


TEST(HierarchicalLevelIndex, toString)
{
   ASSERT_EQ(HierarchicalLevelIndex(2, 3).toString(), "{2, 3}");
}

TEST(HierarchicalLevelIndex, LeftRight_UnahppyPath)
{
   HierarchicalLevelIndex li = HierarchicalLevelIndex(0, 0);
   ASSERT_THROW(li.left(), MyException);
   ASSERT_THROW(li.left(), MyException);

   li = HierarchicalLevelIndex(0, 1);
   ASSERT_THROW(li.left(), MyException);
   ASSERT_THROW(li.left(), MyException);
}

TEST(HierarchicalLevelIndex, LeftRight_1_1)
{
   const  HierarchicalLevelIndex li(1, 1);
   ASSERT_TRUE(li.left() == HierarchicalLevelIndex(0, 0));
   ASSERT_TRUE(li.right() == HierarchicalLevelIndex(0, 1));
}


TEST(HierarchicalLevelIndex, LeftRight_2_1)
{
   const  HierarchicalLevelIndex li(2, 1);
   ASSERT_TRUE(li.left() == HierarchicalLevelIndex(0, 0));
   ASSERT_TRUE(li.right() == HierarchicalLevelIndex(1, 1));
}

TEST(HierarchicalLevelIndex, LeftRight_2_3)
{
   const  HierarchicalLevelIndex li(2, 3);
   ASSERT_TRUE(li.left() == HierarchicalLevelIndex(1, 1));
   ASSERT_TRUE(li.right() == HierarchicalLevelIndex(0, 1));
}


TEST(HierarchicalLevelIndex, LeftRight_3_1)
{
   const  HierarchicalLevelIndex li(3, 1);
   ASSERT_TRUE(li.left() == HierarchicalLevelIndex(0, 0));
   ASSERT_TRUE(li.right() == HierarchicalLevelIndex(2, 1));
}

TEST(HierarchicalLevelIndex, LeftRight_3_3)
{
   const  HierarchicalLevelIndex li(3, 3);
   ASSERT_TRUE(li.left() == HierarchicalLevelIndex(2, 1));
   ASSERT_TRUE(li.right() == HierarchicalLevelIndex(1, 1));
}


TEST(HierarchicalLevelIndex, LeftRight_3_5)
{
   const  HierarchicalLevelIndex li(3, 5);
   ASSERT_TRUE(li.left() == HierarchicalLevelIndex(1, 1));
   ASSERT_TRUE(li.right() == HierarchicalLevelIndex(2, 3));
}

TEST(HierarchicalLevelIndex, LeftRight_3_7)
{
   const  HierarchicalLevelIndex li(3, 7);
   ASSERT_TRUE(li.left() == HierarchicalLevelIndex(2, 3));
   ASSERT_TRUE(li.right() == HierarchicalLevelIndex(0, 1));
}

TEST(HierarchicalLevelIndex, LeftRight_4_7)
{
   const  HierarchicalLevelIndex li(4, 7);
   ASSERT_TRUE(li.left() == HierarchicalLevelIndex(3, 3));
   ASSERT_TRUE(li.right() == HierarchicalLevelIndex(1, 1));
}


TEST(HierarchicalLevelIndex, ToDouble)
{
   ASSERT_NEAR( HierarchicalLevelIndex(0,0).toDouble(), 0.0, Epsilon);
   ASSERT_NEAR(HierarchicalLevelIndex(3, 1).toDouble(), 0.125, Epsilon);
   ASSERT_NEAR(HierarchicalLevelIndex(3, 3).toDouble(), 0.375, Epsilon);
   ASSERT_NEAR(HierarchicalLevelIndex(3, 5).toDouble(), 0.625, Epsilon);
   ASSERT_NEAR(HierarchicalLevelIndex(3, 7).toDouble(), 0.875, Epsilon);
}

