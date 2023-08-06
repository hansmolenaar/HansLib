#include <gtest/gtest.h>

#include "IntervalTreeIndex.h"

using namespace IntervalTree;

namespace
{
   void Check(const Interval<Rational>& intv, Level expectLevel, int expectPositionInLevel, FlyWeightKey expectKey)
   {
      const Index1 indx(intv);
      ASSERT_EQ(indx.getLevel(), expectLevel);
      ASSERT_EQ(indx.getPositionInLevel(), expectPositionInLevel);
      ASSERT_EQ(indx.getKey(), expectKey);
   }
}
TEST(IntervalTreeIndexTest, Level0)
{
   Check(Interval<Rational>(Rational(0, 10), Rational(10, 10)), 0, 0, 0);
}


TEST(IntervalTreeIndexTest, Level1)
{
   Check(Interval<Rational>(Rational(0, 1), Rational(1, 2)), 1, 0, 1);
   Check(Interval<Rational>(Rational(1, 2), Rational(2, 2)), 1, 1, 2);
}


TEST(IntervalTreeIndexTest, Level2)
{
   Check(Interval<Rational>(Rational(0, 2), Rational(1, 4)), 2, 0, 3);
   Check(Interval<Rational>(Rational(1, 4), Rational(1, 2)), 2, 1, 4);
   Check(Interval<Rational>(Rational(2, 4), Rational(3, 4)), 2, 2, 5);
   Check(Interval<Rational>(Rational(3, 4), Rational(4, 4)), 2, 3, 6);
}


TEST(IntervalTreeIndexTest, Level3)
{
   Check(Interval<Rational>(Rational(0, 2), Rational(1, 8)), 3, 0, 7);
   Check(Interval<Rational>(Rational(1, 8), Rational(2, 8)), 3, 1, 8);
   Check(Interval<Rational>(Rational(2, 8), Rational(3, 8)), 3, 2, 9);
   Check(Interval<Rational>(Rational(3, 8), Rational(4, 8)), 3, 3, 10);

   Check(Interval<Rational>(Rational(4, 8), Rational(5, 8)), 3, 4, 11);
   Check(Interval<Rational>(Rational(5, 8), Rational(6, 8)), 3, 5, 12);
   Check(Interval<Rational>(Rational(6, 8), Rational(7, 8)), 3, 6, 13);
   Check(Interval<Rational>(Rational(7, 8), Rational(8, 8)), 3, 7, 14);
}


TEST(IntervalTreeIndexTest, UnhappyPaths)
{
   ASSERT_ANY_THROW(Index1(Interval<Rational>(Rational(-1, 4), Rational(0, 2))));
   ASSERT_ANY_THROW(Index1(Interval<Rational>(Rational(1, 4), Rational(3, 4))));
   ASSERT_ANY_THROW(Index1(Interval<Rational>(Rational(3, 5), Rational(4, 5))));
}


TEST(IntervalTreeIndexTest, Factory)
{
   Index1FlyWeightFactory factory;

   const Index1 indx(Interval<Rational>(Rational(7, 8), Rational(8, 8)));
   const auto key = factory.get(indx);
   ASSERT_EQ(key, indx.getKey());
   const auto& found = factory(key);
   ASSERT_EQ(found.getLevel() , indx.getLevel());
   ASSERT_EQ(found.getPositionInLevel(), indx.getPositionInLevel());
}