#include <gtest/gtest.h>

#include "BoundingBox/BoundingBox.h"
#include "Point.h" 

TEST(IntervalTest, Basic1)
{
   const auto bb = BoundingBox<int, 1>::Create(std::vector<int>{ 42 });
   const auto& intv = bb.getInterval(0);
   ASSERT_EQ(intv.getLower(), 42);
   ASSERT_EQ(intv.getUpper(), 42);
}

TEST(IntervalTest, Basic2)
{
   const auto bb = BoundingBox<int, 2>::Create(std::vector<int>{ 42, 43 });

   auto intv = bb.getInterval(0);
   ASSERT_EQ(intv.getLower(), 42);
   ASSERT_EQ(intv.getUpper(), 42);

   intv = bb.getInterval(1);
   ASSERT_EQ(intv.getLower(), 43);
   ASSERT_EQ(intv.getUpper(), 43);
}

TEST(IntervalTest, Add)
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

TEST(IntervalTest, CreateFromList1)
{
   std::vector< std::vector<int>> values{ {5},{2},{3} };
   const auto bb = BoundingBox<int, 1>::CreateFromList(values);
   ASSERT_EQ(bb.getInterval(0).getLower(), 2);
   ASSERT_EQ(bb.getInterval(0).getUpper(), 5);
}

TEST(IntervalTest, CreateFromList2)
{
   std::vector< std::array<int, 2>> values{ {3,0},{1,2} };
   const auto bb = BoundingBox<int, 2>::CreateFromList(values);
   ASSERT_EQ(bb.getInterval(0).getLower(), 1);
   ASSERT_EQ(bb.getInterval(0).getUpper(), 3);
   ASSERT_EQ(bb.getInterval(1).getLower(), 0);
   ASSERT_EQ(bb.getInterval(1).getUpper(), 2);
}


TEST(IntervalTest, CreateFromListTransformed)
{
   std::vector< std::array<short, 1>> values{ {2},{1} };
   const auto bb = BoundingBox<int, 1>::CreateFromListTransformed(values, [](std::array<short, 1> n) {return std::array<int, 1>{ 2 * n[0] }; });
   ASSERT_EQ(bb.getInterval(0).getLower(), 2);
   ASSERT_EQ(bb.getInterval(0).getUpper(), 4);
}

TEST(IntervalTest, FromPoints)
{
   std::vector<IntPoint1> values{ {2},{1} };
   const auto bb = BoundingBox<int, 1>::CreateFromList(values);
   ASSERT_EQ(bb.getInterval(0).getLower(), 1);
   ASSERT_EQ(bb.getInterval(0).getUpper(), 2);
}

TEST(IntervalTest, LowerUpper)
{
   const auto bb = BoundingBox<int, 2>::CreateFromList(std::initializer_list<IntPoint2>{ {2, 4}, { 3,1 } });
   ASSERT_EQ(bb.getLower(), IntPoint2({ 2, 1 }));
   ASSERT_EQ(bb.getUpper(), IntPoint2({ 3, 4 }));
}


TEST(IntervalTest, Contains)
{
   const auto bb = BoundingBox<int, 2>::CreateFromList(std::initializer_list<IntPoint2>{ {2, 4}, { 3,1 } });
   ASSERT_TRUE(bb.contains(IntPoint2({ 2,4 })));
   ASSERT_FALSE(bb.contains(IntPoint2({ 2,6 })));
   ASSERT_FALSE(bb.contains(IntPoint2({ 6,1 })));
}

TEST(IntervalTest, LowerUpperDim)
{
   const auto bb = BoundingBox<int, 2>::CreateFromList(std::initializer_list<IntPoint2>{ {2, 4}, { 3,1 } });
   ASSERT_EQ(bb.getLower(0), 2);
   ASSERT_EQ(bb.getUpper(0), 3);
   ASSERT_EQ(bb.getLower(1), 1);
   ASSERT_EQ(bb.getUpper(1), 4);
}
