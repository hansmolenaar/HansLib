#include <gtest/gtest.h>

#include "StdHash.h"
#include "Point.h"

#include <numeric>
#include <numbers>
TEST(PointTest, PointArray)
{
   const auto p = Point<int, 1>({ 42 });
   ASSERT_EQ(p.at(0), 42);
   ASSERT_EQ(p.size(), 1);
   ASSERT_EQ(p[0], 42);
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
   const IntPoint3 sum = p1 + p2;
   ASSERT_EQ(sum, IntPoint3({ 9,8, 5 }));
}



TEST(PointTest, MinusIs)
{
   IntPoint3 p1{ 2, 3, 4 };
   const IntPoint3 p2{ 7, 5, 1 };
   p1 = p1 - p2;
   ASSERT_EQ(p1, IntPoint3({ -5, -2, 3 }));
}

TEST(PointTest, PlusAssign)
{
   IntPoint3 p1{ 2, 3, 4 };
   const IntPoint3 p2{ 7, 5, 1 };
   p1 = p1 + p2;
   ASSERT_EQ(p1, IntPoint3({ 9,8, 5 }));
}

TEST(PointTest, Subscript)
{
   const IntPoint2 p{ 1,2 };
   ASSERT_EQ(p.data()[0], 1);
   ASSERT_EQ(p.data()[1], 2);
}


TEST(PointTest, Times)
{
   IntPoint2 p{ 1,2 };
   const IntPoint2 p2 = p * 2;
   ASSERT_EQ(p2.data()[0], 2);
   ASSERT_EQ(p2.data()[1], 4);
}

TEST(PointTest, Bounds)
{
   const IntPoint2 p{ 1,2 };
   int i;
   ASSERT_ANY_THROW(i = p.at(2));
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
   const RatPoint1 p1{ Rational{1, 2} };
   const size_t hash1 = std::hash<RatPoint1>{}(p1);
   const RatPoint1 p2{ Rational{2, 1} };
   const size_t hash2 = std::hash<RatPoint1>{}(p2);
   ASSERT_NE(hash1, hash2);
}


TEST(PointTest, HashRatPoint2)
{
   const RatPoint2 p1{ Rational{1, 2}, Rational{2,1} };
   const size_t hash1 = std::hash<RatPoint2>{}(p1);
   const RatPoint2 p2{ Rational{2, 1}, Rational{1,2} };
   const size_t hash2 = std::hash<RatPoint2>{}(p2);
   ASSERT_NE(hash1, hash2);
}

TEST(PointTest, HashRatPoint3)
{
   const RatPoint3 p1{ Rational{1,1}, Rational{1, 3},Rational {2,1} };
   const size_t hash1 = std::hash<RatPoint3>{}(p1);
   const RatPoint3 p2{ Rational{1,1}, Rational{2, 1}, Rational{1,3} };
   const size_t hash2 = std::hash<RatPoint3>{}(p2);
   ASSERT_NE(hash1, hash2);
}

TEST(PointTest, GetNormSquared)
{
   const IntPoint3 p{ 2, 3, -6 };
   ASSERT_EQ(PointUtils::GetNormSquared(p), 49);
}

TEST(PointTest, Angle)
{
   const Point2 p0{ 1.0, 1.0 };
   const Point2 p1{ 2.0, 1.0 };
   const Point2 p2{ 1.0, 1.0 + std::sqrt(3.0) };

   ASSERT_NEAR(PointUtils::Angle(p1, p0, p2), std::numbers::pi / 2, 1.0e-12);
   ASSERT_NEAR(PointUtils::Angle(p0, p1, p2), std::numbers::pi / 3, 1.0e-12);
   ASSERT_NEAR(PointUtils::Angle(p0, p2, p1), std::numbers::pi / 6, 1.0e-12);
   ASSERT_NEAR(PointUtils::Angle(p0, p2, p0), 0.0, 1.0e-12);
}