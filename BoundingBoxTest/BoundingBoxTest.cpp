#include <gtest/gtest.h>

#include "BoundingBox.h"
#include "Point.h" 

TEST(BoundingBoxTest, Basic1)
{
   const auto bb = BoundingBox<int, 1>::Create(std::vector<int>{ 42 });
   const auto& intv = bb.getInterval(0);
   ASSERT_EQ(intv.getLower(), 42);
   ASSERT_EQ(intv.getUpper(), 42);
}

TEST(BoundingBoxTest, Basic2)
{
   const auto bb = BoundingBox<int, 2>::Create(std::vector<int>{ 42, 43 });

   auto intv = bb.getInterval(0);
   ASSERT_EQ(intv.getLower(), 42);
   ASSERT_EQ(intv.getUpper(), 42);

   intv = bb.getInterval(1);
   ASSERT_EQ(intv.getLower(), 43);
   ASSERT_EQ(intv.getUpper(), 43);
}

TEST(BoundingBoxTest, Add)
{
   auto bb = BoundingBox<int, 1>::Create(std::vector<int>{ 42 });
   const auto& intv = bb.getInterval(0);
   ASSERT_EQ(intv.getLower(), 42);
   ASSERT_EQ(intv.getUpper(), 42);
   bb.Add(std::array<int, 1>{ 1 });
   ASSERT_EQ(intv.getLower(), 1);
   ASSERT_EQ(intv.getUpper(), 42);
   bb.Add(std::array<int, 1>{ 43 });
   ASSERT_EQ(intv.getLower(), 1);
   ASSERT_EQ(intv.getUpper(), 43);
}

TEST(BoundingBoxTest, CreateFromList1)
{
   std::vector< std::vector<int>> values{ {5},{2},{3} };
   const auto bb = BoundingBox<int, 1>::CreateFromList(values);
   ASSERT_EQ(bb.getInterval(0).getLower(), 2);
   ASSERT_EQ(bb.getInterval(0).getUpper(), 5);
}


TEST(BoundingBoxTest, CreateFrom2Points)
{
   const auto bb = BoundingBox<int, 2>::CreateFrom2Points(IntPoint2{ 3, 0 }, IntPoint2{ 1, 2 });
   ASSERT_EQ(bb.getInterval(0).getLower(), 1);
   ASSERT_EQ(bb.getInterval(0).getUpper(), 3);
   ASSERT_EQ(bb.getInterval(1).getLower(), 0);
   ASSERT_EQ(bb.getInterval(1).getUpper(), 2);
}

TEST(BoundingBoxTest, CreateFromList2)
{
   std::vector< std::array<int, 2>> values{ {3,0},{1,2} };
   const auto bb = BoundingBox<int, 2>::CreateFromList(values);
   ASSERT_EQ(bb.getInterval(0).getLower(), 1);
   ASSERT_EQ(bb.getInterval(0).getUpper(), 3);
   ASSERT_EQ(bb.getInterval(1).getLower(), 0);
   ASSERT_EQ(bb.getInterval(1).getUpper(), 2);
}


TEST(BoundingBoxTest, CreateFromListTransformed)
{
   std::vector< std::array<short, 1>> values{ {2},{1} };
   const auto bb = BoundingBox<int, 1>::CreateFromListTransformed(values, [](std::array<short, 1> n) {return std::array<int, 1>{ 2 * n[0] }; });
   ASSERT_EQ(bb.getInterval(0).getLower(), 2);
   ASSERT_EQ(bb.getInterval(0).getUpper(), 4);
}

TEST(BoundingBoxTest, FromPoints)
{
   std::vector<IntPoint1> values{ IntPoint1{2},IntPoint1{1} };
   const auto bb = BoundingBox<int, 1>::CreateFromList(values);
   ASSERT_EQ(bb.getInterval(0).getLower(), 1);
   ASSERT_EQ(bb.getInterval(0).getUpper(), 2);
}

TEST(BoundingBoxTest, LowerUpper)
{
   const auto bb = BoundingBox<int, 2>::CreateFromList(std::initializer_list<IntPoint2>{ IntPoint2{ 2, 4 }, IntPoint2{ 3,1 } });
   ASSERT_EQ(bb.getLower(), IntPoint2({ 2, 1 }));
   ASSERT_EQ(bb.getUpper(), IntPoint2({ 3, 4 }));
}


TEST(BoundingBoxTest, Contains)
{
   const auto bb = BoundingBox<int, 2>::CreateFromList(std::initializer_list<IntPoint2>{ IntPoint2{ 2, 4 }, IntPoint2{ 3,1 } });
   ASSERT_TRUE(bb.contains(IntPoint2({ 2,4 })));
   ASSERT_FALSE(bb.contains(IntPoint2({ 2,6 })));
   ASSERT_FALSE(bb.contains(IntPoint2({ 6,1 })));
}

TEST(BoundingBoxTest, LowerUpperDim)
{
   const auto bb = BoundingBox<int, 2>::CreateFromList(std::initializer_list<IntPoint2>{ IntPoint2{ 2, 4 }, IntPoint2{ 3,1 } });
   ASSERT_EQ(bb.getLower(0), 2);
   ASSERT_EQ(bb.getUpper(0), 3);
   ASSERT_EQ(bb.getLower(1), 1);
   ASSERT_EQ(bb.getUpper(1), 4);
}


TEST(BoundingBoxTest, Intersection)
{
   const auto bb1 = BoundingBox<int, 2>::CreateFromList(std::initializer_list<IntPoint2>{ IntPoint2{ 2, 4 }, IntPoint2{ 5,0 } });
   const auto bb2 = BoundingBox<int, 2>::CreateFromList(std::initializer_list<IntPoint2>{ IntPoint2{ 3, 7 }, IntPoint2{ 0,2 } });
   const auto bb3 = BoundingBox<int, 2>::CreateFromList(std::initializer_list<IntPoint2>{ IntPoint2{ 10, 11 }, IntPoint2{ 12,13 } });
   const auto [succes12, bb12] = BoundingBox<int, 2>::TryGetOverlap(bb1, bb2);
   ASSERT_TRUE(succes12);
   const auto expect = BoundingBox<int, 2>::CreateFromList(std::initializer_list<IntPoint2>{ IntPoint2{ 2, 2 }, IntPoint2{ 3,4 } });
   ASSERT_EQ(bb12, expect);

   const auto [succes13, bb13] = BoundingBox<int, 2>::TryGetOverlap(bb1, bb3);
   ASSERT_FALSE(succes13);
}


TEST(BoundingBoxTest, GetIntervals)
{
   const auto bb = BoundingBox<int, 1>::CreateFromList(std::initializer_list<IntPoint1>{ IntPoint1{ 2 }, IntPoint1{ 1 } });
   ASSERT_EQ(bb.getIntervals().size(), 1);
   ASSERT_EQ(bb.getIntervals()[0].getLower(), 1);
   ASSERT_EQ(bb.getIntervals()[0].getUpper(), 2);
}


TEST(BoundingBoxTest, Scale1)
{
   const auto bb = BoundingBox<int, 1>::CreateFromList(std::initializer_list<IntPoint1>{ IntPoint1{ 2 }, IntPoint1{ 1 } });
   const auto scale = BoundingBox<int, 1>::CreateFromList(std::initializer_list<IntPoint1>{ IntPoint1{ 5 }, IntPoint1{ 10 } });
   const auto result = bb.scaleFrom01(scale);
   ASSERT_EQ(result.getIntervals().size(), 1);
   ASSERT_EQ(result.getIntervals()[0].getLower(), 6);
   ASSERT_EQ(result.getIntervals()[0].getUpper(), 11);
}

TEST(BoundingBoxTest, Scale2)
{
   const auto bb = BoundingBox<Rational, 2>::CreateFromList(std::initializer_list<RatPoint2>{ RatPoint2{ Rational{0}, Rational{2} }, RatPoint2{ Rational{ 1 }, Rational{ 4 } } });
   const auto scale = BoundingBox<Rational, 2>::CreateFrom2Points(RatPoint2{ Rational{1,5}, Rational{2,5} }, RatPoint2{ Rational{ 3,5 }, Rational{ 4,5 } });
   const auto result = bb.scaleFrom01(scale);
   ASSERT_EQ(bb.getIntervals().size(), 2);
   const auto lwr = result.getLower();
   const auto upr = result.getUpper();
   ASSERT_EQ(lwr[0], Rational(1, 5));
   ASSERT_EQ(upr[0], Rational(3, 5));
   ASSERT_EQ(lwr[1], Rational(14, 5));
   ASSERT_EQ(upr[1], Rational(18, 5));
}


TEST(BoundingBoxTest, Scale1Double)
{
   const auto bb = BoundingBox<double, 1>::CreateFrom2Points(Point1{ 10.0 }, Point1{ 12.0 });
   const auto scale = BoundingBox<Rational, 1>::CreateFrom2Points(RatPoint1{ Rational{ 1,4 } }, RatPoint1{ Rational{ 3,4 } });
   const auto result = bb.scaleFrom01(scale);
   ASSERT_EQ(result.getIntervals().size(), 1);
   ASSERT_EQ(result.getIntervals()[0].getLower(), 10.5);
   ASSERT_EQ(result.getIntervals()[0].getUpper(), 11.5);
}

