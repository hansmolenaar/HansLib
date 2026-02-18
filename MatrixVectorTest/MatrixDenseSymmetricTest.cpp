#include <gtest/gtest.h>

#include "Defines.h"
#include "Functors.h"
#include "IMatrixTest.h"
#include "IMatrixUtils.h"
#include "Iota.h"
#include "MatrixDenseSymmetric.h"

#include <array>
#include <iostream>
#include <numbers>
#include <span>

void CheckEigenSolver(MatrixDenseSymmetric &matrix, std::initializer_list<double> expectedEigenvalues = {})
{
    const auto solution = matrix.getEigenSolution();
    ASSERT_TRUE(solution.hasEigenVectors());
    const auto dim = matrix.GetDimension();
    ASSERT_EQ(dim, solution.getAvailableEigenValues().size());

    const Functors::AreClose areClose{.AbsTolerance = 1.0e-10};
    for (auto n : Iota::GetRange(dim))
    {
        const double eigenValue = solution.getAvailableEigenValues()[n];
        if (expectedEigenvalues.size() > 0)
        {
            ASSERT_TRUE(areClose(*(expectedEigenvalues.begin() + n), eigenValue));
        }
        const auto eigenVector = solution.getNthEigenVector(n);
        std::vector<double> matTimesVector(dim);
        std::vector<double> lamTimesVector(eigenVector.begin(), eigenVector.end());

        matrix.timesVector(eigenVector, matTimesVector);
        std::transform(lamTimesVector.begin(), lamTimesVector.end(), lamTimesVector.begin(),
                       [eigenValue](double v) { return v * eigenValue; });

        ASSERT_TRUE(areClose(matTimesVector, lamTimesVector));
    }
}

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
    matrix.set(1, 1, 5.0);
    std::vector<double> vecin{1, 2};
    std::array<double, 2> vecout;
    matrix.timesVector(vecin, vecout);
    ASSERT_NEAR(vecout.at(0), 5.0, 1.0e-10);
    ASSERT_NEAR(vecout.at(1), 12.0, 1.0e-10);

    // Round trip
    str::fill(vecin, -1.0);
    const bool succes = matrix.Solve(vecout, vecin);
    ASSERT_TRUE(succes);
    ASSERT_NEAR(vecin.at(0), 1.0, 1.0e-10);
    ASSERT_NEAR(vecin.at(1), 2.0, 1.0e-10);
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
    const bool succes = matrix.Solve(vecout, vecin);
    ASSERT_TRUE(succes);
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
    const bool succes = matrix.Solve(rhs, sol);
    ASSERT_TRUE(succes);
    ASSERT_NEAR(sol.at(0), 3.0, 1.0e-10);
    ASSERT_NEAR(sol.at(1), -2.0, 1.0e-10);
}

TEST(MatrixDenseSymmetricTest, singular)
{
    MatrixDenseSymmetric matrix(2);
    matrix.set(0, 0, 1.0);
    matrix.set(0, 1, 2.0);
    matrix.set(1, 1, 4.0);
    std::vector<double> sol{-1, -1};
    std::array<double, 2> rhs{5, 10};
    // In range of operator => infinitely many solutions
    bool succes = matrix.Solve(rhs, sol);
    ASSERT_TRUE(succes);

    // Not in range => no solution
    sol = {-1, -1};
    rhs = {4, 10};
    succes = matrix.Solve(rhs, sol);
    ASSERT_FALSE(succes);
}

TEST(MatrixDenseSymmetricTest, eigensolver_1)
{
    constexpr double eps = 1.0e-12;
    MatrixDenseSymmetric matrix(3);
    matrix.set(0, 0, 2.0);
    matrix.set(1, 1, 1.0);
    matrix.set(2, 2, 3.0);

    const auto eigenSolution = matrix.getEigenSolution();
    ASSERT_NEAR(eigenSolution.getAvailableEigenValues()[0], 1.0, eps);
    ASSERT_NEAR(eigenSolution.getAvailableEigenValues()[1], 2.0, eps);
    ASSERT_NEAR(eigenSolution.getAvailableEigenValues()[2], 3.0, eps);

    const Functors::AreClose areClose;
    ASSERT_TRUE(areClose(eigenSolution.getNthEigenVector(0), std::vector<double>{0, 1, 0}));
    ASSERT_TRUE(areClose(eigenSolution.getNthEigenVector(1), std::vector<double>{1, 0, 0}));
    ASSERT_TRUE(areClose(eigenSolution.getNthEigenVector(2), std::vector<double>{0, 0, 1}));
}

TEST(MatrixDenseSymmetricTest, eigensolver_2)
{
    MatrixDenseSymmetric matrix(2);
    matrix.set(0, 0, 1.0);
    matrix.set(1, 1, 1.0);
    matrix.set(0, 1, 2.0);

    CheckEigenSolver(matrix, {-1.0, 3.0});
}

TEST(MatrixDenseSymmetricTest, eigensolver_3)
{
    MatrixDenseSymmetric matrix(2);
    matrix.set(0, 0, 2.0);
    matrix.set(1, 1, 2.0);
    matrix.set(0, 1, 1.0);

    CheckEigenSolver(matrix, {1.0, 3.0});
}

TEST(MatrixDenseSymmetricTest, eigensolver_4)
{
    MatrixDenseSymmetric matrix(3);
    matrix.set(0, 0, 2.0);
    matrix.set(1, 1, 3.0);
    matrix.set(2, 2, 9.0);
    matrix.set(2, 1, 4.0);

    CheckEigenSolver(matrix, {1.0, 2.0, 11.0});
}

TEST(MatrixDenseSymmetricTest, eigensolver_5)
{
    MatrixDenseSymmetric matrix(3);
    SetAll(matrix, 2.0);
    CheckEigenSolver(matrix, {0.0, 0.0, 6.0});
}

TEST(MatrixDenseSymmetricTest, eigensolver_6)
{
    MatrixDenseSymmetric matrix(3);
    matrix.set(0, 0, 2.0);
    matrix.set(1, 1, 2.0);
    matrix.set(2, 2, 2.0);
    matrix.set(0, 1, -1.0);
    matrix.set(2, 1, -1.0);

    CheckEigenSolver(matrix, {2 - std::numbers::sqrt2, 2.0, 2 + std::numbers::sqrt2});
}

TEST(MatrixDenseSymmetricTest, eigensolver_7)
{
    MatrixDenseSymmetric matrix(3);
    matrix.set(0, 1, 1.0);
    matrix.set(0, 2, 1.0);
    matrix.set(1, 2, 1.0);

    CheckEigenSolver(matrix, {-1, -1, 2});
}

TEST(MatrixDenseSymmetricTest, eigensolver_8)
{
    MatrixDenseSymmetric matrix(2);
    matrix.set(0, 0, 1.0);
    matrix.set(1, 1, 1.0);
    matrix.set(0, 1, -0.5);

    CheckEigenSolver(matrix, {0.5, 1.5});
}

TEST(MatrixDenseSymmetricTest, eigensolver_9)
{
    // ( n  n )
    // ( n  n ) => 0 and 2n
    MatrixDenseSymmetric matrix(4);
    matrix.set(0, 0, 2.0);
    matrix.set(1, 1, 2.0);
    matrix.set(0, 1, 2.0);

    matrix.set(2, 2, 3.0);
    matrix.set(3, 3, 3.0);
    matrix.set(2, 3, 3.0);
    CheckEigenSolver(matrix, {0.0, 0.0, 4.0, 6.0});
}

TEST(MatrixDenseSymmetricTest, eigensolver_10)
{
    // ( 4n  2n )
    // ( 2n   n ) => 0 and 5n
    MatrixDenseSymmetric matrix(4);
    matrix.set(0, 0, 4.0);
    matrix.set(1, 1, 1.0);
    matrix.set(0, 1, 2.0);

    matrix.set(2, 2, 8.0);
    matrix.set(3, 3, 2.0);
    matrix.set(2, 3, 4.0);
    CheckEigenSolver(matrix, {0.0, 0.0, 5.0, 10.0});
}

TEST(MatrixDenseSymmetricTest, eigensolver_11)
{
    MatrixDenseSymmetric matrix(4);
    matrix.set(0, 0, 1.0);
    matrix.set(1, 1, 9.0);
    matrix.set(2, 2, 4.0);
    matrix.set(3, 3, 1.0);

    matrix.set(0, 1, -3.0);
    matrix.set(0, 2, 2.0);
    matrix.set(0, 3, -1.0);
    matrix.set(1, 2, -6.0);
    matrix.set(1, 3, 3.0);
    matrix.set(2, 3, -2.0);
    CheckEigenSolver(matrix, {0.0, 0.0, 0.0, 15.0});
}

TEST(MatrixDenseSymmetricTest, eigensolver_12)
{
    // integer matrix with integer eigenvalues
    MatrixDenseSymmetric matrix(5);
    // [ 1 -1  2  2 -1;
    //  -1 -1  2  1  1;
    //   2  2 -2  2  2;
    //   2  1  2 -1  1;
    //  -1  1  2  1 -1]
    matrix.set(0, 0, 1);
    matrix.set(0, 1, -1);
    matrix.set(0, 2, 2);
    matrix.set(0, 3, 2);
    matrix.set(0, 4, -1);

    matrix.set(1, 1, -1);
    matrix.set(1, 2, 2);
    matrix.set(1, 3, 1);
    matrix.set(1, 4, 1);

    matrix.set(2, 2, -2);
    matrix.set(2, 3, 2);
    matrix.set(2, 4, 2);

    matrix.set(3, 3, -1);
    matrix.set(3, 4, 1);

    matrix.set(4, 4, -1);

    CheckEigenSolver(matrix, {-5.0, -3.0, -2.0, 2.0, 4.0});
}

TEST(MatrixDenseSymmetricTest, eigensolver_13)
{
    // integer matrix with integer eigenvalues
    MatrixDenseSymmetric matrix(3);
    matrix.set(0, 0, 2);
    matrix.set(0, 1, 2);
    matrix.set(0, 2, 2);

    matrix.set(1, 1, -2);
    matrix.set(2, 2, 1);

    CheckEigenSolver(matrix, {-3.0, 0.0, 4.0});
}
