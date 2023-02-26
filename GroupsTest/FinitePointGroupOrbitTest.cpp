#include <gtest/gtest.h>

#include "FinitePointGroupOrbit.h"
#include "ActionReflectOrigin.h"
#include "PointClose.h"
#include "DihedralGroup.h"
#include "ActionDihedral.h"

TEST(FinitePointGroupOrbitTest, Basic1)
{
   const ActionReflectOrigin<1> reflect;
   const PointClose<double, 1> areClose;
   const Point1 p{ 2.0 };
   const Point1 pr{ -2.0 };
   const FinitePointGroupOrbit<1> orbit(reflect, p, areClose);
   ASSERT_EQ(orbit.getNumPoints(), 2);
   ASSERT_TRUE(areClose.SamePoints(orbit.getPoint(0), p));
   ASSERT_TRUE(areClose.SamePoints(orbit.getPoint(1), pr));

   auto found = orbit.tryGetClosePoint(pr);
   ASSERT_TRUE(std::get<0>(found));
   ASSERT_EQ(std::get<1>(found), 1);

   found = orbit.tryGetClosePoint(Point1{ 1 });
   ASSERT_FALSE(std::get<0>(found));
}

TEST(FinitePointGroupOrbitTest, Di3)
{
   const ActionDihedral di(4);
   const PointClose<double, 2> areClose;
   const Point2 p{ 2, 1 };
   const FinitePointGroupOrbit<2> orbit(di, p, areClose);

   ASSERT_EQ(orbit.getNumPoints(), 8);
}
