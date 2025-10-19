#include <gtest/gtest.h>

#include "MathConstants.h"
#include "PointClose.h"
#include "UnitVector.h"

#include <numeric>

namespace
{
   constexpr double crit = 1.0e-10;
}

TEST(UnitVectorTest, Norm)
{
   const Point2 p{ {3,-4} };
   const auto uv = UnitVector<double, GeomDim2>::Create(p);

   ASSERT_NEAR(uv[0], 0.6, crit);
   ASSERT_NEAR(uv[1], -0.8, crit);
}


TEST(UnitVectorTest, Null)
{
   const Point2 p{ {0,0} };
   const auto uv = UnitVector<double, GeomDim2>::Create(p);
   ASSERT_FALSE(uv.isValid());
}

TEST(UnitVectorTest, Bounds)
{
   const Point1 p{ {2} };
   const auto uv = UnitVector<double, GeomDim1>::Create(p);
   ASSERT_NEAR(uv[0], 1.0, crit);
   double val;
   ASSERT_ANY_THROW(val = uv[1]);
}

TEST(UnitVectorTest, InnerProduct)
{
   const Point2 p{ {2,0} };
   const auto uv = UnitVector<double, GeomDim2>::Create(std::vector<double>{1, 1});
   const double ip = uv.innerProduct(p);
   ASSERT_DOUBLE_EQ(ip, std::sqrt(2.0));
   ASSERT_DOUBLE_EQ(uv * uv, 1.0);
}

TEST(UnitVectorTest, TimesFactor)
{
   const auto uv = UnitVector<double, GeomDim2>::Create(Point2{ 3,4 });
   const PointClose<double, GeomDim2> areClose;
   ASSERT_TRUE(areClose(uv * 2.0, Point2{ 1.2, 1.6 }));
   ASSERT_TRUE(areClose(2.0 * uv, Point2{ 1.2, 1.6 }));
}

TEST(UnitVectorTest, CreateUnhappyPath)
{
   auto uv = UnitVector<double, GeomDim1>::Create(Point1{ 3 });
   ASSERT_MYEXCEPTION_MESSAGE((uv = UnitVector<double, 1>::Create(std::vector<double>{ 1.0, 2.0 })), "UnitVector<N>::Create span dimension incorrect");
}


TEST(UnitVectorTest, CreateFromDirectedPair)
{
   const Point2 from{ 1, -2 };
   const Point2 to{ 4, -6 };

   auto uv = UnitVector<double, GeomDim2>::Create(from, to);
   ASSERT_NEAR(uv[0], 0.6, crit);
   ASSERT_EQ(uv[1], -0.8);
}

TEST(UnitVectorTest, TestUnitVector2)
{
   std::array<double, 2> data = { 1,-1 };
   const auto vec = UnitVector<double, 2>::Create(data);
   ASSERT_TRUE(vec.isValid());

   ASSERT_DOUBLE_EQ(vec[0], MathConstants::SQRT1_2);
   ASSERT_DOUBLE_EQ(vec[1], -MathConstants::SQRT1_2);
}

TEST(UnitVectorTest, TestUnitVectorThrow)
{
   std::array<double, 2> data = { 0,0 };
   const auto vec = UnitVector<double, 2>::Create(data);
   ASSERT_FALSE(vec.isValid());
}

TEST(UnitVectorTest, InnerProduct2)
{
   std::array<double, 2> data = { 1,0 };
   const auto vec = UnitVector<double, 2>::Create(data);
   ASSERT_DOUBLE_EQ(vec * vec, 1.0);
}
