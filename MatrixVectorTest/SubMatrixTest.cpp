#include <gtest/gtest.h>

#include "Defines.h"
#include "MatrixDense.h"
#include "MatrixSquare.h"
#include "SubMatrix.h"

#include <numeric>
TEST(SubMatrixTest, Test1)
{
    const double eps = 1.0e-10;
    const int dim = 8;
    MatrixSquare mat(dim);
    for (int row : str::iota_view(0, dim))
    {
        for (int col : str::iota_view(0, dim))
        {
            mat(row, col) = row * 0.1 + col * 0.01;
        }
    }

    SubMatrix submat(mat, 2, 3, 1, 2);
    ASSERT_NEAR(submat.get(0, 0), 0.12, eps);
    ASSERT_NEAR(submat.get(0, 1), 0.13, eps);
    ASSERT_NEAR(submat.get(0, 2), 0.14, eps);
    ASSERT_NEAR(submat.get(1, 0), 0.22, eps);
    ASSERT_NEAR(submat.get(1, 1), 0.23, eps);
    ASSERT_NEAR(submat.get(1, 2), 0.24, eps);

    submat.set(1, 1, 1.0);
    ASSERT_NEAR(mat(2, 3), 1.0, eps);

    ASSERT_ANY_THROW(submat.get(2, 0););
    ASSERT_ANY_THROW(submat.get(0, 3););
}

TEST(SubMatrixTest, TestErrors)
{
    const int dim = 8;
    MatrixSquare mat(dim);

    ASSERT_ANY_THROW(SubMatrix(mat, -1, 0, 0, 0););
    ASSERT_ANY_THROW(SubMatrix(mat, 0, -1, 0, 0););
    ASSERT_ANY_THROW(SubMatrix(mat, 0, 0, -1, 0););
    ASSERT_ANY_THROW(SubMatrix(mat, 0, 0, 0, -1););

    ASSERT_NO_THROW(SubMatrix(mat, 7, 7, 1, 1););
    ASSERT_ANY_THROW(SubMatrix(mat, 7, 7, 1, 2););
    ASSERT_ANY_THROW(SubMatrix(mat, 7, 7, 2, 1););
}

TEST(SubMatrixTest, timesVector)
{
    constexpr int rdim = 4;
    constexpr int cdim = 3;
    MatrixDense mat(rdim, cdim);
    mat(1, 0) = 1;
    mat(1, 1) = 2;
    mat(2, 0) = 3;
    mat(2, 1) = 4;

    const SubMatrix subMatrix(mat, 2, 2, 1, 0);
    std::array<double, 2> vecin{1, 2};
    std::array<double, 2> vecout;
    subMatrix.timesVector(vecin, vecout);

    ASSERT_NEAR(vecout.at(0), 5.0, 1.0e-10);
    ASSERT_NEAR(vecout.at(1), 11.0, 1.0e-10);
}
