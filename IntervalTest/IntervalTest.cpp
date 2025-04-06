#include <gtest/gtest.h>

#include "Functors.h"
#include "Interval.h"
#include <sstream>

TEST(IntervalTest, Basic1)
{
   const Interval<int> interval(42);
   ASSERT_EQ(42, interval.getLower());
   ASSERT_EQ(42, interval.getUpper());
}


TEST(IntervalTest, Basic2)
{
   const Interval<int> interval(2, 1);
   ASSERT_EQ(1, interval.getLower());
   ASSERT_EQ(2, interval.getUpper());
}


TEST(IntervalTest, Add)
{
   Interval<int> interval(1, 3);
   interval.add(2);
   ASSERT_EQ(1, interval.getLower());
   ASSERT_EQ(3, interval.getUpper());
   interval.add(0);
   ASSERT_EQ(0, interval.getLower());
   ASSERT_EQ(3, interval.getUpper());
   interval.add(4);
   ASSERT_EQ(0, interval.getLower());
   ASSERT_EQ(4, interval.getUpper());
}


TEST(IntervalTest, ContainsNumber)
{
   Interval<int> interval(1, 3);
   ASSERT_FALSE(interval.contains(0));
   ASSERT_TRUE(interval.contains(1));
   ASSERT_TRUE(interval.contains(2));
   ASSERT_TRUE(interval.contains(3));
   ASSERT_FALSE(interval.contains(4));
}

TEST(IntervalTest, ContainsInterval)
{
   Interval<int> interval(0, 3);
   ASSERT_FALSE(interval.contains(Interval<int>(-2, -1)));
   ASSERT_FALSE(interval.contains(Interval<int>(-2, 2)));
   ASSERT_TRUE(interval.contains(Interval<int>(0, 2)));
   ASSERT_TRUE(interval.contains(Interval<int>(1, 2)));
   ASSERT_TRUE(interval.contains(Interval<int>(1, 3)));
   ASSERT_FALSE(interval.contains(Interval<int>(2, 5)));
   ASSERT_FALSE(interval.contains(Interval<int>(3, 5)));
   ASSERT_FALSE(interval.contains(Interval<int>(4, 5)));
}


TEST(IntervalTest, TryIntersect)
{
   Interval<int> interval(0, 3);
   int lo, hi;

   ASSERT_FALSE(Interval<int>::TryIntersect(interval, Interval<int>(-2, -1), lo, hi));
   ASSERT_FALSE(Interval<int>::TryIntersect(interval, Interval<int>(4, 5), lo, hi));

   ASSERT_TRUE(Interval<int>::TryIntersect(interval, Interval<int>(-1, 2), lo, hi));
   ASSERT_TRUE(lo == 0 && hi == 2);

   ASSERT_TRUE(Interval<int>::TryIntersect(interval, Interval<int>(1, 5), lo, hi));
   ASSERT_TRUE(lo == 1 && hi == 3);

   ASSERT_TRUE(Interval<int>::TryIntersect(interval, Interval<int>(1, 2), lo, hi));
   ASSERT_TRUE(lo == 1 && hi == 2);

   ASSERT_TRUE(Interval<int>::TryIntersect(interval, Interval<int>(-1, 5), lo, hi));
   ASSERT_TRUE(lo == 0 && hi == 3);

   ASSERT_TRUE(Interval<int>::TryIntersect(interval, Interval<int>(0, 3), lo, hi));
   ASSERT_TRUE(lo == 0 && hi == 3);
}


TEST(IntervalTest, Measure)
{
   const Interval<int> interval(-1, 3);
   ASSERT_EQ(interval.getMeasure(), 4);
}


TEST(IntervalTest, InterpolateOK)
{
   constexpr Functors::AreClose areClose;
   const Interval<double> interval(-1, 3);
   const double arg = 7;
   const double factor = interval.inverseInterpolate(arg);
   ASSERT_TRUE(areClose(factor, 2.0));
   const double retval = interval.interpolate(factor);
   ASSERT_TRUE(areClose(retval, arg));
}


TEST(IntervalTest, InterpolateFail)
{
   constexpr Functors::AreClose areClose;
   Interval<double> interval(0, 0);
   ASSERT_THROW(interval.inverseInterpolate(1.0), MyException);
   interval = Interval<double>(0, std::numeric_limits<double>::epsilon());
   ASSERT_THROW(interval.inverseInterpolate(std::numeric_limits<double>::max()), MyException);
}

TEST(IntervalTest, Equals)
{
   const Interval<int> interval1(0, 1);
   const Interval<int> interval2(0, 2);
   const Interval<int> interval3(1, 1);
   ASSERT_EQ(interval1, interval1);
   ASSERT_NE(interval1, interval2);
   ASSERT_NE(interval1, interval3);
}

TEST(IntervalTest, StreamInsertion)
{
   const Interval<int> interval(1, 2);
   std::ostringstream os;
   os << interval;
   ASSERT_EQ(os.str(), "(1, 2)");
}