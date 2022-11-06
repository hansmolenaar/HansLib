#include <gtest/gtest.h>

#include "Geometry/Point.h"

#include <numeric>

TEST(PointTest, PointArray)
{
   const auto p = Point<int, 1>({ 42 });
   ASSERT_EQ(p.at(0), 42);
   const auto data = p.data();
   ASSERT_EQ(data.size(), 1);
   ASSERT_EQ(data[0], 42);
}


TEST(PointTest, Iterator)
{
   const IntPoint3 p{ 1, 2, 3 };
   ASSERT_EQ(6, std::accumulate(p.begin(), p.end(), 0));
}


TEST(PointTest, Spaceship)
{
   const IntPoint3 p1{ 1, 2, 3 };
   const IntPoint3 p2{ 1, 2, 4 };
   ASSERT_FALSE(p1 == p2);
   ASSERT_TRUE(p1 != p2);
   ASSERT_TRUE(p1 < p2);
   ASSERT_FALSE(p1 >= p2);
}

TEST(PointTest, Plus)
{
   const IntPoint3 p1{ 2, 3, 4 };
   const IntPoint3 p2{ 7, 5, 1 };
   const auto& sum = p1 + p2;
   ASSERT_EQ(sum, IntPoint3({ 9,8, 5 }));
}

TEST(PointTest, PlusAssign)
{
   IntPoint3 p1{ 2, 3, 4 };
   const IntPoint3 p2{ 7, 5, 1 };
   p1 += p2;
   ASSERT_EQ(p1, IntPoint3({ 9,8, 5 }));
}

TEST(PointTest, Subscript)
{
   IntPoint2 p{ 1,2 };
   p[0] = 3;
   p[1] = 4;
   const IntPoint2& cp = p;
   ASSERT_EQ(cp[0], 3);
   ASSERT_EQ(cp[1], 4);
}


TEST(PointTest, Times)
{
   IntPoint2 p{ 1,2 };
   const IntPoint2 p2 = p * 2;
   ASSERT_EQ(p2[0], 2);
   ASSERT_EQ(p2[1], 4);
}

TEST(PointTest, Bounds)
{
   IntPoint2 p{ 1,2 };
   int i;
   ASSERT_ANY_THROW(i = p[2]);
   ASSERT_ANY_THROW(p[2] = 0);
}

