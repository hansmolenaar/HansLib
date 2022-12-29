#include <gtest/gtest.h>

#include "Interpolation/HierLevelIndex.h"
#include "Utilities/MyException.h"


inline constexpr double Epsilon = 1.0e-10;

TEST(HierLevelIndexTest, Basis)
{
   const  HierLevelIndex li(2, 1);
   ASSERT_EQ(li.getLevel(), 2);
   ASSERT_EQ(li.getIndex(), 1);
}

TEST(HierLevelIndexTest, CheckLevelIndex)
{
   ASSERT_THROW(HierLevelIndex(0, 2), MyException);
   ASSERT_THROW(HierLevelIndex(1, 2), MyException);
   ASSERT_THROW(HierLevelIndex(1, 3), MyException);
   ASSERT_THROW(HierLevelIndex(2, 4), MyException);
   ASSERT_THROW(HierLevelIndex(2, 5), MyException);
   ASSERT_THROW(HierLevelIndex(3, 8), MyException);
   ASSERT_THROW(HierLevelIndex(3, 9), MyException);
}

TEST(HierLevelIndex, SpaceShip)
{
   const  HierLevelIndex li0(2, 1);
   const  HierLevelIndex li1(2, 3);
   ASSERT_TRUE(li0 != li1);
   ASSERT_TRUE(li0 < li1);
}


TEST(HierLevelIndex, toString)
{
   ASSERT_EQ(HierLevelIndex(2, 3).toString(), "{2, 3}");
}


TEST(HierLevelIndex, ToDouble)
{
   ASSERT_NEAR(HierLevelIndex(0, 0).toDouble(), 0.0, Epsilon);
   ASSERT_NEAR(HierLevelIndex(3, 1).toDouble(), 0.125, Epsilon);
   ASSERT_NEAR(HierLevelIndex(3, 3).toDouble(), 0.375, Epsilon);
   ASSERT_NEAR(HierLevelIndex(3, 5).toDouble(), 0.625, Epsilon);
   ASSERT_NEAR(HierLevelIndex(3, 7).toDouble(), 0.875, Epsilon);
}

