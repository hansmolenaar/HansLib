#include <gtest/gtest.h>

#include "Point/Point.h"

#include <numeric>

TEST(PointTest, PointArray)
{
   const auto p = Point<int, 1>({ 42 });
   ASSERT_EQ(p.at(0), 42);
}


TEST(PointTest, Iterator)
{
   const IntPoint3 p{ 1, 2, 3 };
   ASSERT_EQ(6, std::accumulate(p.begin(), p.end(), 0));
}
