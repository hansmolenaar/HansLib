#include <gtest/gtest.h>

#include "IMatrixKelvinReprUtils.h"
#include "MatrixKelvinRepr2.h"
#include "MatrixKelvinRepr3.h"

namespace
{
constexpr double c_eps = 1.0e-12;
}

TEST(IMatrixKelvinReprUtilsTest, TestIdentity)
{
    const auto id = MatrixKelvinRepr3::CreateIdentity();
    ASSERT_EQ(1.0, id(0, 0));
    ASSERT_EQ(1.0, id(1, 1));
    ASSERT_EQ(1.0, id(2, 2));

    ASSERT_EQ(0.0, id(0, 1));
    ASSERT_EQ(0.0, id(0, 2));
    ASSERT_EQ(0.0, id(1, 2));

    ASSERT_EQ(0.0, id(1, 0));
    ASSERT_EQ(0.0, id(2, 0));
    ASSERT_EQ(0.0, id(2, 1));
}

TEST(IMatrixKelvinReprUtilsTest, TestTimes)
{
    auto matrix = MatrixKelvinRepr3::CreateIdentity();
    matrix.Set(0, 2, 2.0);
    matrix.Set(1, 2, 3.0);
    matrix.Set(0, 1, 4.0);

    matrix *= 2;

    ASSERT_NEAR(2.0, matrix.Get(0, 0), c_eps);
    ASSERT_NEAR(2.0, matrix.Get(1, 1), c_eps);
    ASSERT_NEAR(2.0, matrix.Get(2, 2), c_eps);

    ASSERT_NEAR(8.0, matrix.Get(0, 1), c_eps);
    ASSERT_NEAR(4.0, matrix.Get(0, 2), c_eps);
    ASSERT_NEAR(6.0, matrix.Get(1, 2), c_eps);

    ASSERT_NEAR(8.0, matrix.Get(1, 0), c_eps);
    ASSERT_NEAR(4.0, matrix.Get(2, 0), c_eps);
    ASSERT_NEAR(6.0, matrix.Get(2, 1), c_eps);
}

TEST(IMatrixKelvinReprUtilsTest, TestTimes2)
{
    auto matrix = MatrixKelvinRepr2::Create();
    matrix.Set(0, 0, 1.0);
    matrix.Set(0, 1, 2.0);
    matrix.Set(1, 1, 3.0);

    matrix *= 2;

    ASSERT_NEAR(2.0, matrix.Get(0, 0), c_eps);
    ASSERT_NEAR(6.0, matrix.Get(1, 1), c_eps);
    ASSERT_NEAR(4.0, matrix.Get(0, 1), c_eps);
    ASSERT_NEAR(4.0, matrix.Get(1, 0), c_eps);
}

TEST(IMatrixKelvinReprUtilsTest, TestAdd)
{
    auto matrix1 = MatrixKelvinRepr2::Create();
    matrix1.Set(0, 0, 1.0);
    matrix1.Set(0, 1, 2.0);
    matrix1.Set(1, 1, 3.0);

    auto matrix2 = MatrixKelvinRepr2::Create();
    matrix2.Set(0, 0, 2.0);
    matrix2.Set(0, 1, 3.0);
    matrix2.Set(1, 1, 4.0);

    matrix1 += matrix2;

    ASSERT_NEAR(3.0, matrix1.Get(0, 0), c_eps);
    ASSERT_NEAR(7.0, matrix1.Get(1, 1), c_eps);
    ASSERT_NEAR(5.0, matrix1.Get(0, 1), c_eps);
    ASSERT_NEAR(5.0, matrix1.Get(1, 0), c_eps);
}