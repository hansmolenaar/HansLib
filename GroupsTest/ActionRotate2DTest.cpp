#include <gtest/gtest.h>

#include "ActionRotate2D.h"
#include "IFinitePointGroupActionTest.h"
#include "PointClose.h"

TEST(ActionRotate2DTest, Interface)
{
   const ActionRotate2D groupAction(7);
   Test_IFinitePointGroupAction(groupAction, GetRandomPoint<2>());
}


TEST(ActionRotate2DTest, Four)
{
   const ActionRotate2D groupAction(4);
   const Point2 point{ 1,0 };
   const PointClose<double, 2> areClose;

   ASSERT_TRUE(areClose(groupAction(0, point), Point2{ 1,0 }));
   ASSERT_TRUE(areClose(groupAction(1, point), Point2{ 0,1 }));
   ASSERT_TRUE(areClose(groupAction(2, point), Point2{ -1,0 }));
   ASSERT_TRUE(areClose(groupAction(3, point), Point2{ 0,-1 }));
}
