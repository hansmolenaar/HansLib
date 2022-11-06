#include <gtest/gtest.h>

#include "Groups/ActionDihedral.h"
#include "IFinitePointGroupActionTest.h"
#include "Point//PointClose.h"
#include "Groups/IFinitePointGroupActionUtils.h"

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


TEST(ActionDihedralTest, Three)
{
   const Point2 point{ 1,0 };
   const ActionDihedral groupAction(3, point);
   const PointClose<double, 2> areClose;

   const auto orbit = IFinitePointGroupActionUtils::GetOrbit(groupAction, point);

   ASSERT_TRUE(areClose(orbit.at(0), Point2{ 1,0 }));
   ASSERT_TRUE(areClose(orbit.at(1), Point2{ -0.5, 0.5 * std::sqrt(3.0) }));
   ASSERT_TRUE(areClose(orbit.at(2), Point2{ -0.5, -0.5 * std::sqrt(3.0) }));

   ASSERT_TRUE(areClose(orbit.at(3), Point2{ 1,0 }));
   ASSERT_TRUE(areClose(orbit.at(4), Point2{ -0.5, 0.5 * std::sqrt(3.0) }));
   ASSERT_TRUE(areClose(orbit.at(5), Point2{ -0.5, -0.5 * std::sqrt(3.0) }));

   Test_IFinitePointGroupAction(groupAction, point);
}

TEST(ActionDihedralTest, Four1)
{
   const Point2 point{ 1,0 };
   const ActionDihedral groupAction(4);
   const PointClose<double, 2> areClose;

   const auto orbit = IFinitePointGroupActionUtils::GetOrbit(groupAction, point);
   ASSERT_TRUE(areClose(orbit.at(0), Point2{1,0}));
   ASSERT_TRUE(areClose(orbit.at(1), Point2{ 0,1 }));
   ASSERT_TRUE(areClose(orbit.at(2), Point2{ -1,0 }));
   ASSERT_TRUE(areClose(orbit.at(3), Point2{ 0,-1 }));

   ASSERT_TRUE(areClose(orbit.at(4), Point2{ 1,0 }));
   ASSERT_TRUE(areClose(orbit.at(5), Point2{ 0,1 }));
   ASSERT_TRUE(areClose(orbit.at(6), Point2{ -1,0 }));
   ASSERT_TRUE(areClose(orbit.at(7), Point2{ 0,-1 }));
}


TEST(ActionDihedralTest, Four2)
{
   const Point2 point{ 1,1 };
   const ActionDihedral groupAction(4);
   const PointClose<double, 2> areClose;

   const auto orbit = IFinitePointGroupActionUtils::GetOrbit(groupAction, point);
   ASSERT_TRUE(areClose(orbit.at(0), Point2{ 1,1 }));
   ASSERT_TRUE(areClose(orbit.at(1), Point2{ -1,1 }));
   ASSERT_TRUE(areClose(orbit.at(2), Point2{ -1,-1 }));
   ASSERT_TRUE(areClose(orbit.at(3), Point2{ 1,-1 }));

   ASSERT_TRUE(areClose(orbit.at(4), Point2{ 1,-1 }));
   ASSERT_TRUE(areClose(orbit.at(5), Point2{ 1,1 }));
   ASSERT_TRUE(areClose(orbit.at(6), Point2{ -1,1 }));
   ASSERT_TRUE(areClose(orbit.at(7), Point2{ -1,-1 }));
}


TEST(ActionDihedralTest, Interface)
{
   for (int n = 1; n < 8; ++n)
   {
      const Point2 point{ 1,0 };
      const ActionDihedral groupAction(n, point);
      Test_IFinitePointGroupAction(groupAction, point);
   }
}
