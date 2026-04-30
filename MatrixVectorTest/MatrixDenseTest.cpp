#include <gtest/gtest.h>

#include "IMatrixTest.h"
#include "MatrixDense.h"

TEST(MatrixDenseTest, TestConstructor)
{
    MatrixDense mat(2, 3);
    ASSERT_EQ(2, mat.GetRowDimension());
    ASSERT_EQ(3, mat.GetColDimension());
}

TEST(MatrixDenseTest, TestInterface)
{
    MatrixDense mat(2, 3);
    IMatrixTest::TestInterface(mat, true);
}

TEST(MatrixDenseTest, TestGetSet)
{
    const int nRow = 5;
    const int nCol = 7;
    MatrixDense mat(nRow, nCol);
    double val = 0.0;
    for (int row = 0; row < nRow; ++row)
    {
        for (int col = 0; col < nCol; ++col)
        {
            val += 1.0;
            mat(row, col) = val;
        }
    }

    val = 0.0;
    for (int row = 0; row < nRow; ++row)
    {
        for (int col = 0; col < nCol; ++col)
        {
            val += 1.0;
            ASSERT_EQ(val, mat(row, col));
        }
    }
}

TEST(MatrixDenseTest, TimesVector)
{
    const int nRow = 2;
    const int nCol = 3;
    MatrixDense mat(nRow, nCol);
    double val = 0.0;
    for (int row = 0; row < nRow; ++row)
    {
        for (int col = 0; col < nCol; ++col)
        {
            val += 1.0;
            mat(row, col) = val;
        }
    }

    const std::vector<double> vecin{1, 2, 3};
    std::vector<double> vecout(2);
    mat.timesVector(vecin, vecout);
    ASSERT_NEAR(vecout.at(0), 14.0, 1.0e-10);
    ASSERT_NEAR(vecout.at(1), 32.0, 1.0e-10);
}
