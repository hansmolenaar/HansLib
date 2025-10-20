#include <gtest/gtest.h>

#include "IMatrixSquareUtils.h"
#include "IMatrixTest.h"
#include "IMatrixUtils.h"
#include "MathConstants.h"
#include "MatrixRotation3D.h"
#include "UnitVector.h"

namespace
{
const double eps = 1.0e-12;
}

TEST(MatrixRotation3DTest, TestBase)
{
    const auto axisz = UnitVector<double, 3>::Create(std::array<double, 3>({0, 0, 1}));

    const auto &mat = MatrixRotation3D::Create(axisz, MathConstants::PI_4);
    ASSERT_EQ(3, mat.GetRowDimension());
    ASSERT_EQ(3, mat.GetColDimension());

    ASSERT_TRUE(IMatrixRotation::IsRotation(mat));

    ASSERT_NEAR(mat(0, 0), MathConstants::SQRT1_2, eps);
    ASSERT_NEAR(mat(1, 1), MathConstants::SQRT1_2, eps);
    ASSERT_NEAR(mat(0, 1), -MathConstants::SQRT1_2, eps);
    ASSERT_NEAR(mat(1, 0), MathConstants::SQRT1_2, eps);
}

TEST(MatrixRotation3DTest, TestUnit)
{
    const auto axisz = UnitVector<double, 3>::Create(std::array<double, 3>({1, 1, 1}));
    const auto &mat = MatrixRotation3D::Create(axisz, 0.0);

    ASSERT_TRUE(IMatrixRotation::IsRotation(mat));

    ASSERT_NEAR(mat(0, 0), 1.0, eps);
    ASSERT_NEAR(mat(1, 1), 1.0, eps);
    ASSERT_NEAR(mat(2, 2), 1.0, eps);
    ASSERT_NEAR(mat(0, 1), 0.0, eps);
    ASSERT_NEAR(mat(1, 0), 0.0, eps);
    ASSERT_NEAR(mat(0, 2), 0.0, eps);
    ASSERT_NEAR(mat(2, 0), 0.0, eps);
    ASSERT_NEAR(mat(1, 2), 0.0, eps);
    ASSERT_NEAR(mat(2, 1), 0.0, eps);
}

TEST(MatrixRotation3DTest, TestPermute)
{
    const auto axisz = UnitVector<double, 3>::Create(std::array<double, 3>({1, 1, 1}));
    const auto &mat = MatrixRotation3D::Create(axisz, 2 * MathConstants::PI / 3);

    const std::array<double, 3> vecin = {1, 2, 3};
    std::vector<double> vecout(3);

    MatrixTimesVector(mat, vecin, vecout);

    ASSERT_NEAR(vecout[0], 3.0, eps);
    ASSERT_NEAR(vecout[1], 1.0, eps);
    ASSERT_NEAR(vecout[2], 2.0, eps);
}