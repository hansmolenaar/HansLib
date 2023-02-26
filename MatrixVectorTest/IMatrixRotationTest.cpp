#include <gtest/gtest.h>

#include "IMatrixRotation.h"
#include "IMatrixSquareUtils.h"
#include "MatrixSquare.h"
#include "IdentityMatrix.h"
#include "UnitVector.h"

namespace {
   const double eps = 1.0e-12;
}


TEST(IMatrixRotationTest, TestIsRotation1)
{
   const int dim = 3;
   const IdentityMatrix matrix(dim);
   ASSERT_TRUE(IsOrthogonal(matrix));
   ASSERT_TRUE(IMatrixRotation::IsRotation(matrix));
}


TEST(IMatrixRotationTest, TestIsRotation2)
{
   MatrixSquare matrix(2);
   matrix(0, 1) = 1;
   matrix(1, 0) = 1;
   ASSERT_TRUE(IsOrthogonal(matrix));
   ASSERT_FALSE(IMatrixRotation::IsRotation(matrix));
}


TEST(IMatrixRotationTest, TestIsRotation3)
{
   const double cosx = std::cos(0.1);
   const double sinx = std::sin(0.1);
   MatrixSquare matrix(3);
   matrix(0, 0) = 1;
   matrix(1, 1) = cosx;
   matrix(2, 2) = cosx;
   matrix(1, 2) = -sinx;
   matrix(2, 1) = sinx;
   ASSERT_TRUE(IsOrthogonal(matrix));
   ASSERT_TRUE(IMatrixRotation::IsRotation(matrix));
}


