#include <gtest/gtest.h>

#include "IMatrixKelvinReprTest.h"
#include "IMatrixSquareUtils.h"
#include "MatrixKelvinRepr1.h"

TEST(MatrixKelvinRepr1Test, TestInterface)
{
    MatrixKelvinRepr1 matrix;
    ASSERT_EQ(1, matrix.GetDimension());
    IMatrixKelvinReprTest::TestInterface(matrix);
}
