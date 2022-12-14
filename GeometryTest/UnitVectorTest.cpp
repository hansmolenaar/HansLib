#include <gtest/gtest.h>

#include "Geometry/UnitVector.h"
#include "Geometry/PointClose.h"

#include <numeric>

namespace
{
   constexpr double crit = 1.0e-10;
}

TEST(UnitVectorTest, Norm)
{
   const Point2 p{ {3,-4} };
   const auto uv = UnitVector<2>::Create(p);

   ASSERT_NEAR((*uv)[0], 0.6, crit);
   ASSERT_NEAR((*uv)[1], -0.8, crit);
}


TEST(UnitVectorTest, Null)
{
   const Point2 p{ {0,0} };
   const auto uv = UnitVector<2>::Create(p);
   ASSERT_FALSE(uv);
}

TEST(UnitVectorTest, Bounds)
{
   const Point1 p{ {2} };
   const auto uv = UnitVector<1>::Create(p);
   ASSERT_NEAR((*uv)[0], 1.0, crit);

   ASSERT_ANY_THROW((*uv)[1]);
}

TEST(UnitVectorTest, InnerProduct)
{
   const Point2 p{ {2,0} };
   const auto uv = UnitVector<2>::Create(std::vector<double>{1, 1});
   const double ip = uv->innerProduct(p);
   ASSERT_NEAR(ip, std::sqrt(2.0), crit);
}


TEST(UnitVectorTest, TimesFactor)
{
   const auto uv = UnitVector<2>::Create(Point2{ 3,4 });
   const PointClose<double, 2> areClose;
   ASSERT_TRUE(areClose((*uv) * 2.0, Point2{ 1.2, 1.6 }));
   ASSERT_TRUE(areClose(2 * (*uv), Point2{ 1.2, 1.6 }));
}


TEST(UnitVectorTest, CreateUnhappyPath)
{
   ASSERT_THROW(UnitVector<1>::Create(std::vector<double>{ 1,2 }), MyException);
}