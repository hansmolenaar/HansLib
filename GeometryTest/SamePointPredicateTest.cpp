#include <gtest/gtest.h>

#include "SamePointPredicate.h"
#include "PointClose.h"

#include <numbers>

TEST(SamePointPredicateTest, Basic)
{
   const PointClose<double, 1> areClose;
   const SamePointPredicate<double, 1> samePoint{ areClose, Point1{1.0} };
   ASSERT_TRUE(samePoint(Point1{ 1.0 }));
   ASSERT_FALSE(samePoint(Point1{ 2.0 }));
}
