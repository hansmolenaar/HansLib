#include <gtest/gtest.h>

#include "IMatrixTest.h"
#include "IMatrixUtils.h"
#include "MatrixSquare.h"

#include <array>
#include <span>

TEST(MatrixSquareTest, Test1)
{
    MatrixSquare matrix(1);
    ASSERT_EQ(1, matrix.GetDimension());
    ASSERT_EQ(1, matrix.GetRowDimension());
    ASSERT_EQ(1, matrix.GetColDimension());
    matrix(0, 0) = 2.0;
    ASSERT_EQ(matrix(0, 0), 2.0);
    const auto rhs = std::array<double, 1>{6.0};
    auto sol = std::array<double, 1>();
    bool succes = matrix.Solve(rhs, sol);
    ASSERT_TRUE(succes);
    ASSERT_EQ(sol[0], 3.0);
}

TEST(MatrixSquareTest, TestInterface)
{
    for (int dim = 1; dim < 5; ++dim)
    {
        MatrixSquare matrix(dim);
        IMatrixTest::TestInterface(matrix);
    }
}

TEST(MatrixSquareTest, Test2)
{
    MatrixSquare matrix(1);
    const auto rhs = std::array<double, 1>{6.0};
    auto sol = std::array<double, 1>();
    ASSERT_FALSE(matrix.Solve(rhs, sol));
}

TEST(MatrixSquareTest, Test3)
{
    MatrixSquare matrix(2);
    ASSERT_EQ(2, matrix.GetDimension());
    matrix(0, 0) = 1.0;
    matrix(0, 1) = 2.0;
    matrix(1, 0) = 3.0;
    matrix(1, 1) = 4.0;

    ASSERT_EQ(matrix(0, 0), 1.0);
    ASSERT_EQ(matrix(0, 1), 2.0);
    ASSERT_EQ(matrix(1, 0), 3.0);
    ASSERT_EQ(matrix(1, 1), 4.0);
    const auto rhs = std::array<double, 2>{5, 11};
    auto sol = std::array<double, 2>();
    bool succes = matrix.Solve(rhs, sol);
    ASSERT_TRUE(succes);
    ASSERT_EQ(sol[0], 1.0);
    ASSERT_EQ(sol[1], 2.0);
}

TEST(MatrixSquareTest, Test4)
{
    MatrixSquare matrix(3);
    matrix(0, 0) = 1.0;
    matrix(1, 1) = 2.0;
    matrix(2, 2) = 3.0;
    matrix(2, 0) = 4.0;
    const auto rhs = std::array<double, 3>{1, 4, 13};
    auto sol = std::array<double, 3>();
    bool succes = matrix.Solve(rhs, sol);
    ASSERT_TRUE(succes);
    ASSERT_EQ(sol[0], 1.0);
    ASSERT_EQ(sol[1], 2.0);
    ASSERT_EQ(sol[2], 3.0);
}

// Another singular matrix
TEST(MatrixSquareTest, Test5)
{
    MatrixSquare matrix(2);
    matrix(0, 0) = 1.0;
    matrix(0, 1) = -1.0;
    matrix(1, 0) = -2.0;
    matrix(1, 1) = 2.0;
    const auto rhs = std::array<double, 2>{6, 1};
    auto sol = std::array<double, 2>();
    ASSERT_FALSE(matrix.Solve(rhs, sol));
}

// Zeroes on main diagonal
TEST(MatrixSquareTest, Test6)
{
    MatrixSquare matrix(2);
    matrix(0, 1) = 1.0;
    matrix(1, 0) = 2.0;
    const auto rhs = std::array<double, 2>{4, 6};
    auto sol = std::array<double, 2>();
    bool succes = matrix.Solve(rhs, sol);
    ASSERT_TRUE(succes);
    ASSERT_EQ(sol[0], 3.0);
    ASSERT_EQ(sol[1], 4.0);
}

// Data/Indexer dirty
TEST(MatrixSquareTest, TestDirty)
{
    MatrixSquare matrix(1);

    matrix(0, 0) = 1.0;

    std::vector<double> sol(1);
    std::vector<double> rhs = {2.0};

    matrix.Solve(rhs, sol);
    ASSERT_EQ(sol[0], 2.0);

    matrix(0, 0) = 2.0;
    matrix.Solve(rhs, sol);
    ASSERT_EQ(sol[0], 1.0);
}

// Add
TEST(MatrixSquareTest, TestAdd)
{
    MatrixSquare matrix(1);
    matrix(0, 0) = 1.0;
    ASSERT_EQ(matrix(0, 0), 1.0);
    matrix(0, 0) += 1.0;
    ASSERT_EQ(matrix(0, 0), 2.0);
}

// Clear
TEST(MatrixSquareTest, TestClear)
{
    MatrixSquare matrix(1);
    matrix(0, 0) = 1.0;
    ASSERT_EQ(matrix(0, 0), 1.0);
    Clear(matrix);
    ASSERT_EQ(matrix(0, 0), 0.0);
}
