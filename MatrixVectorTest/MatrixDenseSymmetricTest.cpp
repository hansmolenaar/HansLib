#include <gtest/gtest.h>

#include "Defines.h"
#include "IMatrixTest.h"
#include "IMatrixUtils.h"
#include "MatrixDenseSymmetric.h"

#include <array>
#include <iostream>
#include <span>

TEST(MatrixDenseSymmetricTest, Test1)
{
    MatrixDenseSymmetric matrix(1);
    ASSERT_EQ(1, matrix.GetDimension());
    ASSERT_EQ(1, matrix.GetRowDimension());
    ASSERT_EQ(1, matrix.GetColDimension());
    matrix.set(0, 0, 2.0);
    ASSERT_EQ(matrix(0, 0), 2.0);
}

TEST(MatrixDenseSymmetricTest, TestInterface)
{
    for (int dim = 1; dim < 5; ++dim)
    {
        MatrixDenseSymmetric matrix(dim);
        IMatrixTest::TestInterface(matrix);
    }
}

TEST(MatrixDenseSymmetricTest, Test3)
{
    MatrixDenseSymmetric matrix(2);
    ASSERT_EQ(2, matrix.GetDimension());
    matrix.set(0, 0, 1.0);
    matrix.set(0, 1, 2.0);
    matrix.set(1, 0, 3.0);
    matrix.set(1, 1, 4.0);

    ASSERT_EQ(matrix(0, 0), 1.0);
    ASSERT_EQ(matrix(0, 1), 3.0);
    ASSERT_EQ(matrix(1, 0), 3.0);
    ASSERT_EQ(matrix(1, 1), 4.0);
}

TEST(MatrixDenseSymmetricTest, timesVector2)
{
    MatrixDenseSymmetric matrix(2);
    matrix.set(0, 0, 1.0);
    matrix.set(0, 1, 2.0);
    matrix.set(1, 1, 4.0);
    const std::vector<double> vecin{1, 2};
    std::array<double, 2> vecout;
    matrix.timesVector(vecin, vecout);
    ASSERT_NEAR(vecout.at(0), 5.0, 1.0e-10);
    ASSERT_NEAR(vecout.at(1), 10.0, 1.0e-10);
}

TEST(MatrixDenseSymmetricTest, timesVector3)
{
    MatrixDenseSymmetric matrix(3);
    matrix.set(0, 0, 1.0);
    matrix.set(0, 1, 2.0);
    matrix.set(0, 2, 3.0);
    matrix.set(1, 1, 4.0);
    matrix.set(1, 2, 5.0);
    matrix.set(2, 2, 6.0);

    std::vector<double> vecin{3, 2, 1};
    std::array<double, 3> vecout;
    matrix.timesVector(vecin, vecout);
    ASSERT_NEAR(vecout.at(0), 10.0, 1.0e-10);
    ASSERT_NEAR(vecout.at(1), 19.0, 1.0e-10);
    ASSERT_NEAR(vecout.at(2), 25.0, 1.0e-10);

    // Round trip
    str::fill(vecin, -1.0);
    matrix.Solve(vecout, vecin);
    ASSERT_NEAR(vecin.at(0), 3.0, 1.0e-10);
    ASSERT_NEAR(vecin.at(1), 2.0, 1.0e-10);
    ASSERT_NEAR(vecin.at(2), 1.0, 1.0e-10);
}

TEST(MatrixDenseSymmetricTest, solveIndefinite)
{
    MatrixDenseSymmetric matrix(2);
    matrix.set(0, 0, 1.0);
    matrix.set(1, 1, -1.0);

    const std::vector<double> rhs{3, 2};
    std::array<double, 2> sol;
    matrix.Solve(rhs, sol);
    ASSERT_NEAR(sol.at(0), 3.0, 1.0e-10);
    ASSERT_NEAR(sol.at(1), -2.0, 1.0e-10);
}
