#include <gtest/gtest.h>

#include "BoundingBox/BoundingBox.h"

TEST(IntervalTest, Basic1)
{
   const BoundingBox<int, 1> bb(std::vector<int>{ 42 });
   const auto& intv = bb.getInterval(0);
   ASSERT_EQ(intv.getLower(), 42);
   ASSERT_EQ(intv.getUpper(), 42);
}


TEST(IntervalTest, Basic2)
{
   const BoundingBox<int, 2> bb(std::vector<int>{ 42, 43 });

   auto intv = bb.getInterval(0);
   ASSERT_EQ(intv.getLower(), 42);
   ASSERT_EQ(intv.getUpper(), 42);

   intv = bb.getInterval(1);
   ASSERT_EQ(intv.getLower(), 43);
   ASSERT_EQ(intv.getUpper(), 43);
}
