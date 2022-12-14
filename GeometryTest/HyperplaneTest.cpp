#include <gtest/gtest.h>

#include "Geometry/Hyperplane.h"
#include "Geometry/PointClose.h"


namespace
{
   constexpr double crit = 1.0e-10;
}

TEST(HyperplaneTest, getSignedDistance)
{
   const Hyperplane<1> hp(Point1{ 3 }, UnitVector<1>::Create(Point1{ 2 }));

   ASSERT_NEAR(hp.getSignedDistance(Point1{ 7 }), 4.0, crit);
   ASSERT_NEAR(hp.getSignedDistance(Point1{ -7 }), -10.0, crit);
}


TEST(HyperplaneTest, reflect)
{
   const Hyperplane<2> hp(Point2{ 0,0 }, UnitVector<2>::Create(Point2{ -1,1 }));

   const PointClose<double, 2> areClose;
   ASSERT_TRUE(areClose(hp.reflect(Point2{ -1, 0 }), Point2{ 0,-1 }));
   ASSERT_TRUE(areClose(hp.reflect(Point2{ 2, 2 }), Point2{ 2,2 }));
   ASSERT_TRUE(areClose(hp.reflect(Point2{ -2, 2 }), Point2{ 2,-2 }));
}


TEST(HyperplaneTest, reflect2)
{
   const Hyperplane<1> hp(Point1{ 2 }, UnitVector<1>::Create(Point1{ -1 }));

   const PointClose<double, 1> areClose;
   ASSERT_TRUE(areClose(hp.reflect(Point1{ 0 }), Point1{ 4 }));
   ASSERT_TRUE(areClose(hp.reflect(Point1{ 2 }), Point1{ 2 }));
   ASSERT_TRUE(areClose(hp.reflect(Point1{ 4 }), Point1{ 0 }));
}

TEST(HyperplaneTest, reflect3)
{
   const Hyperplane<2> hp(Point2{ 1,0 }, UnitVector<2>::Create(Point2{ 1,1 }));

   const PointClose<double, 2> areClose;
   ASSERT_TRUE(areClose(hp.reflect(Point2{ 2,2 }), Point2{ -1,-1 }));
   ASSERT_TRUE(areClose(hp.reflect(Point2{ 1,0 }), Point2{ 1,0 }));
   ASSERT_TRUE(areClose(hp.reflect(Point2{ -1,-1 }), Point2{ 2,2 }));
}
