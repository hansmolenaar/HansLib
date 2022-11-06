#include <gtest/gtest.h>

#include "Groups/ActionDihedral.h"
#include "IFinitePointGroupActionTest.h"
#include "Point//PointClose.h"

TEST(ActionDihedralTest, Two)
{
   const Point2 point{ 2,1 };
   const ActionDihedral groupAction(2);
   const PointClose<double, 2> areClose;

   ASSERT_TRUE(areClose(groupAction(0, point), Point2{ 2,1 }));
   ASSERT_TRUE(areClose(groupAction(1, point), Point2{ -2,-1 }));
   ASSERT_TRUE(areClose(groupAction(2, point), Point2{ 2,-1 }));
   ASSERT_TRUE(areClose(groupAction(3, point), Point2{ -2,1 }));
}
