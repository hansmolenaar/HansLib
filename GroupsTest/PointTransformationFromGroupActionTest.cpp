#include <gtest/gtest.h>

#include "Groups/PointTransformationFromGroupAction.h"
#include "Groups/CyclicGroup.h"
#include "Groups/ActionRotate2D.h"
#include "PointClose.h"

TEST(PointTransformationFromGroupActionTest, Trivial)
{
   const ActionRotate2D groupAction(4);
   const Point2 point{ 2,0 };
   const IFinitePointGroupAction<2>& tmp = groupAction;
   const PointTransformationFromGroupAction<2> ga{ groupAction, 2 };
   const auto retval = ga(point);
   const PointClose<double, 2> areClose;
   ASSERT_TRUE(areClose(retval, Point2{ -2, 0 }));
};
