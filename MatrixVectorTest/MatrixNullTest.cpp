#include <gtest/gtest.h>

#include "IMatrixTest.h"
#include "MatrixNull.h"

TEST(MatrixNullTest, TestNull)
{
    MatrixNull mat;
    ASSERT_EQ(0, mat.GetRowDimension());
    ASSERT_EQ(0, mat.GetColDimension());
    ASSERT_ANY_THROW(mat(0, 0) = 0.0;);
    ASSERT_ANY_THROW(mat(0, 0););
}

TEST(MatrixNullTest, TestInterface)
{
    MatrixNull mat;
    IMatrixTest::TestConstInterface(mat);
}
