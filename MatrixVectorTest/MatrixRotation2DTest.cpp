#include <gtest/gtest.h>

#include "IMatrixSquareUtils.h"
#include "MatrixRotation2D.h"
#include "IMatrixTest.h"
#include "MathConstants.h"

#include <array>

namespace {
   const double eps = 1.0e-12;
}

TEST(MatrixRotation2DTest, TestNull)
{
   const MatrixRotation2D mat(MathConstants::PI_4);
   ASSERT_EQ(2, mat.GetRowDimension());
   ASSERT_EQ(2, mat.GetColDimension());

   ASSERT_TRUE(IMatrixRotation::IsRotation(mat));

   ASSERT_NEAR(mat(0, 0), MathConstants::SQRT1_2, eps);
   ASSERT_NEAR(mat(1, 1), MathConstants::SQRT1_2, eps);
   ASSERT_NEAR(mat(0, 1), -MathConstants::SQRT1_2, eps);
   ASSERT_NEAR(mat(1, 0), MathConstants::SQRT1_2, eps);
}

TEST(MatrixRotation2DTest, TestTransform)
{
   const MatrixRotation2D mat(MathConstants::PI_2);
   std::array < double, 2 > vec0 = { 1,0 };
   std::array < double, 2 > vec1;

   mat.Transform(vec0, vec1);
   ASSERT_NEAR(vec1[0], 0.0, eps);
   ASSERT_NEAR(vec1[1], 1.0, eps);

   mat.Transform(vec1, vec0);
   ASSERT_NEAR(vec0[0], -1.0, eps);
   ASSERT_NEAR(vec0[1],0.0, eps);

   mat.Transform(vec0, vec1);
   ASSERT_NEAR(vec1[0], 0.0, eps);
   ASSERT_NEAR(vec1[1], -1.0, eps);

   mat.Transform(vec1, vec0);
   ASSERT_NEAR(vec0[0], 1.0, eps);
   ASSERT_NEAR(vec0[1], 0.0, eps);
}







