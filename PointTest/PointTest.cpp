#include <gtest/gtest.h>

#include "Point.h"

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



TEST(PointTest, MinusIs)
{
   IntPoint3 p1{ 2, 3, 4 };
   const IntPoint3 p2{ 7, 5, 1 };
   p1 -= p2;
   ASSERT_EQ(p1, IntPoint3({ -5, -2, 3 }));
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
   const IntPoint2 p{ 1,2 };
   ASSERT_EQ(p[0], 1);
   ASSERT_EQ(p[1], 2);
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
}

TEST(PointTest, HashIntPoint1)
{
   const IntPoint1 p1{ 42 };
   const size_t hash1 = std::hash<IntPoint1>{}(p1);
   const IntPoint1 p2{ 43 };
   const size_t hash2 = std::hash<IntPoint1>{}(p2);
   ASSERT_NE(hash1, hash2);
}


TEST(PointTest, HashIntPoint2)
{
   const IntPoint2 p1{ 42, 1 };
   const size_t hash1 = std::hash<IntPoint2>{}(p1);
   const IntPoint2 p2{ 1, 42 };
   const size_t hash2 = std::hash<IntPoint2>{}(p2);
   ASSERT_NE(hash1, hash2);
}

TEST(PointTest, HashIntPoint3)
{
   const IntPoint3 p1{ 1, 2, 3 };
   const size_t hash1 = std::hash<IntPoint3>{}(p1);
   const IntPoint3 p2{ 1, 3, 2 };
   const size_t hash2 = std::hash<IntPoint3>{}(p2);
   ASSERT_NE(hash1, hash2);
}

TEST(PointTest, HashRatPoint1)
{
   const RatPoint1 p1{ {1, 2} };
   const size_t hash1 = std::hash<RatPoint1>{}(p1);
   const RatPoint1 p2{ {2, 1} };
   const size_t hash2 = std::hash<RatPoint1>{}(p2);
   ASSERT_NE(hash1, hash2);
}


TEST(PointTest, HashRatPoint2)
{
   const RatPoint2 p1{ {1, 2}, {2,1} };
   const size_t hash1 = std::hash<RatPoint2>{}(p1);
   const RatPoint2 p2{ {2, 1}, {1,2} };
   const size_t hash2 = std::hash<RatPoint2>{}(p2);
   ASSERT_NE(hash1, hash2);
}

TEST(PointTest, HashRatPoint3)
{
   const RatPoint3 p1{ {1,1}, {1, 3}, {2,1} };
   const size_t hash1 = std::hash<RatPoint3>{}(p1);
   const RatPoint3 p2{ {1,1}, {2, 1}, {1,3} };
   const size_t hash2 = std::hash<RatPoint3>{}(p2);
   ASSERT_NE(hash1, hash2);
}