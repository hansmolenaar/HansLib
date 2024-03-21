#include <gtest/gtest.h>

#include "Hyperplane.h"
#include "PointClose.h"


namespace
{
   constexpr double crit = 1.0e-10;
}

TEST(HyperplaneTest, getSignedDistance)
{
   const Hyperplane<double, 1> hp(Point1{ 3 }, UnitVector<double,1>::Create(Point1{ 2 }).value());

   ASSERT_NEAR(hp.getSignedDistance(Point1{ 7 }), 4.0, crit);
   ASSERT_NEAR(hp.getSignedDistance(Point1{ -7 }), -10.0, crit);
}


TEST(HyperplaneTest, reflect)
{
   const Hyperplane<double, 2> hp(Point2{ 0,0 }, UnitVector<double, 2>::Create(Point2{ -1,1 }).value());

   const PointClose<double, 2> areClose;
   ASSERT_TRUE(areClose(hp.reflect(Point2{ -1, 0 }), Point2{ 0,-1 }));
   ASSERT_TRUE(areClose(hp.reflect(Point2{ 2, 2 }), Point2{ 2,2 }));
   ASSERT_TRUE(areClose(hp.reflect(Point2{ -2, 2 }), Point2{ 2,-2 }));
}


TEST(HyperplaneTest, reflect2)
{
   const Hyperplane<double, 1> hp(Point1{ 2 }, UnitVector<double, 1>::Create(Point1{ -1 }).value());

   const PointClose<double, 1> areClose;
   ASSERT_TRUE(areClose(hp.reflect(Point1{ 0 }), Point1{ 4 }));
   ASSERT_TRUE(areClose(hp.reflect(Point1{ 2 }), Point1{ 2 }));
   ASSERT_TRUE(areClose(hp.reflect(Point1{ 4 }), Point1{ 0 }));
}

TEST(HyperplaneTest, reflect3)
{
   const Hyperplane<double, 2> hp(Point2{ 1,0 }, UnitVector<double, 2>::Create(Point2{ 1,1 }).value());

   const PointClose<double, 2> areClose;
   ASSERT_TRUE(areClose(hp.reflect(Point2{ 2,2 }), Point2{ -1,-1 }));
   ASSERT_TRUE(areClose(hp.reflect(Point2{ 1,0 }), Point2{ 1,0 }));
   ASSERT_TRUE(areClose(hp.reflect(Point2{ -1,-1 }), Point2{ 2,2 }));
}
