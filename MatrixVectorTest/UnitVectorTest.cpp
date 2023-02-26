#include <gtest/gtest.h>

#include "MatrixVector/UnitVector.h"
#include "IMatrixTest.h"
#include "MyAssert.h"
#include "MathConstants.h"

namespace {
   const double eps = 1.0e-12;
}

TEST(UnitVectorTest, TestUnitVector2)
{
   std::array<double, 2> data = { 1,-1 };
   const auto vecptr = UnitVector2::TryCreate(data);
   ASSERT_TRUE(vecptr.get() != nullptr);
   ASSERT_EQ(2, vecptr->GetDimension());

   const auto& vec = *vecptr;
   ASSERT_NEAR(vec(0), MathConstants::SQRT1_2, eps);
   ASSERT_NEAR(vec(1), -MathConstants::SQRT1_2, eps);
}


TEST(UnitVectorTest, TestUnitVectorInvalid)
{
   std::array<double, 2> data = { 0,0 };
   const auto vec = UnitVector2::TryCreate(data);
   ASSERT_TRUE(vec.get() == nullptr);
}


TEST(UnitVectorTest, TestUnitVectorThrow)
{
   std::array<double, 2> data = { 1,1 };
   ASSERT_ANY_THROW(UnitVector3::TryCreate(data));
   auto vec = UnitVector2::TryCreate(data);
   ASSERT_ANY_THROW((*vec)(0) = 2.0);
}






