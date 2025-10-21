#include <gtest/gtest.h>

#include "Defines.h"
#include "EigenValueSolverSym3x3.h"
#include "IRealFunctionUtils.h"
#include "IndexerKelvinRepr3.h"
#include "MatrixKelvinRepr3.h"
#include "MyAssert.h"
#include "RealFunctionCheckDerivative.h"

#include <numbers>

namespace
{
constexpr double eps = 1.0e-7;

// Return random number in the range [-1,1]
double RandomNumber()
{
    const int randomInt = std::rand();
    const double randomNormalized = (double)randomInt / RAND_MAX;
    return 2 * (randomNormalized - 0.5);
}

void CheckCalculationSingleMatrix(const MatrixKelvinRepr3 &matrix,
                                  const std::span<const double> &expectedSortedEigenValues, const bool checkDerivatives,
                                  const double del)
{
    const double tol = 1.0e-6;
    std::vector<double> eigenvalues(3);
    EigenValueSolverSym3x3::CalculateEigenvalues3x3(matrix, eigenvalues);
    std::sort(eigenvalues.begin(), eigenvalues.end());
    for (int n = 0; n < 3; ++n)
    {
        const double dif = std::abs(eigenvalues[n] - expectedSortedEigenValues[n]);
        Utilities::MyAssert(std::abs(eigenvalues[n] - expectedSortedEigenValues[n]) < tol);
    }

    ASSERT_NEAR(eigenvalues[0], expectedSortedEigenValues[0], tol);
    ASSERT_NEAR(eigenvalues[1], expectedSortedEigenValues[1], tol);
    ASSERT_NEAR(eigenvalues[2], expectedSortedEigenValues[2], tol);

    if (checkDerivatives)
    {
        EigenValueSolverSym3x3 solver;
        RealFunctionCheckDerivative::CheckExtrapolation(solver, matrix.Vector(), std::vector<double>(6, del));
    }
}

} // namespace

TEST(EigenValueSolverSym3x3Test, TestBasis)
{
    EigenValueSolverSym3x3 solver;
    ASSERT_EQ(3, solver.GetRangeDimension());
    ASSERT_EQ(6, solver.GetDomainDimension());
    ASSERT_TRUE(solver.HasDerivative());
}

TEST(EigenValueSolverSym3x3Test, Identity)
{
    auto mat = MatrixKelvinRepr3::CreateIdentity();

    std::vector<double> eigenvalues(GeomDim3);
    EigenValueSolverSym3x3::CalculateEigenvalues3x3(mat, eigenvalues);

    ASSERT_NEAR(eigenvalues[0], 1, eps);
    ASSERT_NEAR(eigenvalues[1], 1, eps);
    ASSERT_NEAR(eigenvalues[2], 1, eps);

    str::fill(eigenvalues, 0.0);
    EigenValueSolverSym3x3 solver;
    solver.Evaluate(mat.Vector(), eigenvalues);
    ASSERT_NEAR(eigenvalues[0], 1, eps);
    ASSERT_NEAR(eigenvalues[1], 1, eps);

    const std::array<double, 3> expectedSortedEigenValues{1, 1, 1};
    CheckCalculationSingleMatrix(mat, expectedSortedEigenValues, false, 1.0e-3);
}

TEST(EigenValueSolverSym3x3Test, TestEigenvaluesDiagnal)
{
    auto tensor = MatrixKelvinRepr3::CreateEmpty();
    for (int n = 0; n < 3; ++n)
    {
        tensor.Set(n, n, n + 1);
    }
    const std::array<double, 3> eigenValues = {1, 2, 3};

    CheckCalculationSingleMatrix(tensor, eigenValues, false, 1.0e-3);
}

TEST(EigenValueSolverSym3x3Test, TestEigenvalues1)
{
    auto matrix = MatrixKelvinRepr3::CreateEmpty();
    matrix.Set(0, 0, 2.0);
    matrix.Set(1, 1, 2.0);
    matrix.Set(2, 2, 2.0);
    matrix.Set(0, 1, 1.0);
    matrix.Set(1, 2, 1.0);
    const std::array<double, 3> eigenvalues = {2 - std::numbers::sqrt2, 2, 2 + std::numbers::sqrt2};
    CheckCalculationSingleMatrix(matrix, eigenvalues, false, 1.0e-3);
}

TEST(EigenValueSolverSym3x3Test, TestEigenvalues2)
{
    auto matrix = MatrixKelvinRepr3::CreateEmpty();
    matrix.Set(0, 0, 3.0);
    matrix.Set(1, 1, 3.0);
    matrix.Set(2, 2, 5.0);
    matrix.Set(0, 1, 1.0);
    matrix.Set(0, 2, -1.0);
    matrix.Set(1, 2, -1.0);

    const std::array<double, 3> eigenvalues = {2, 3, 6};
    CheckCalculationSingleMatrix(matrix, eigenvalues, false, 1.0e-3);
}

TEST(EigenValueSolverSym3x3Test, TestEigenvalues3)
{
    auto matrix = MatrixKelvinRepr3::CreateEmpty();
    matrix.Set(0, 0, 3.0);
    matrix.Set(1, 1, 2.0);
    matrix.Set(2, 2, 2.0);
    matrix.Set(0, 1, 1.0);
    matrix.Set(0, 2, 1.0);
    matrix.Set(1, 2, 2.0);
    const std::array<double, 3> eigenvalues = {0, 2, 5};
    CheckCalculationSingleMatrix(matrix, eigenvalues, false, 1.0e-3);
}

TEST(EigenValueSolverSym3x3Test, TestEigenvalues5)
{
    auto matrix = MatrixKelvinRepr3::CreateEmpty();
    matrix.Set(0, 0, 0.0);
    matrix.Set(1, 1, -2.0);
    matrix.Set(2, 2, 3.0);
    matrix.Set(0, 1, 0.0);
    matrix.Set(0, 2, -2.0);
    matrix.Set(1, 2, 0.0);
    const std::array<double, 3> eigenvalues = {-2, -1, 4};
    CheckCalculationSingleMatrix(matrix, eigenvalues, false, 1.0e-3);
}

TEST(EigenValueSolverSym3x3Test, TestEigenvalues6)
{
    auto matrix = MatrixKelvinRepr3::CreateEmpty();
    matrix.Set(0, 2, 1.0);
    const std::array<double, 3> eigenvalues = {-1, 0, 1};
    CheckCalculationSingleMatrix(matrix, eigenvalues, false, 1.0e-3);
}

TEST(EigenValueSolverSym3x3Test, TestEigenvalues7)
{
    auto matrix = MatrixKelvinRepr3::CreateEmpty();
    matrix.Set(0, 0, 1.0);
    matrix.Set(1, 1, 1.0);
    matrix.Set(2, 2, 1.0);
    matrix.Set(0, 1, 1.0);
    matrix.Set(1, 2, 1.0);
    const std::array<double, 3> eigenvalues = {1 - MathConstants::SQRT2, 1, 1 + MathConstants::SQRT2};
    CheckCalculationSingleMatrix(matrix, eigenvalues, false, 1.0e-3);
}

TEST(EigenValueSolverSym3x3Test, TestEigenvalues8)
{
    auto matrix = MatrixKelvinRepr3::CreateEmpty();
    matrix.Set(0, 0, -1.0);
    matrix.Set(1, 1, 2.0);
    matrix.Set(2, 2, 2.0);
    matrix.Set(0, 1, 2.0);
    matrix.Set(1, 2, -1.0);
    matrix.Set(0, 2, 2.0);
    const std::array<double, 3> eigenvalues = {-3, 3, 3};
    CheckCalculationSingleMatrix(matrix, eigenvalues, false, 1.0e-3);
}

TEST(EigenValueSolverSym3x3Test, TestTwoIdenticalEigenvalues0)
{
    auto matrix = MatrixKelvinRepr3::CreateEmpty();
    matrix.Set(0, 0, 1.0);
    matrix.Set(1, 1, 1.0);
    matrix.Set(2, 2, 2.0);

    const std::array<double, 3> eigenvalues = {1, 1, 2};
    CheckCalculationSingleMatrix(matrix, eigenvalues, false, 1.0e-3);
}

TEST(EigenValueSolverSym3x3Test, TestTwoIdenticalEigenvalues1)
{
    auto matrix = MatrixKelvinRepr3::CreateEmpty();
    matrix.Set(0, 0, 3.0);
    matrix.Set(1, 1, 0.0);
    matrix.Set(2, 2, 3.0);
    matrix.Set(0, 1, 2.0);
    matrix.Set(0, 2, 4.0);
    matrix.Set(1, 2, 2.0);

    const std::array<double, 3> eigenvalues = {-1, -1, 8};
    CheckCalculationSingleMatrix(matrix, eigenvalues, false, 1.0e-3);
}

TEST(EigenValueSolverSym3x3Test, TestTwoIdenticalEigenvalues2)
{
    auto matrix = MatrixKelvinRepr3::CreateEmpty();
    matrix.Set(0, 0, 0.0);
    matrix.Set(1, 1, 0.0);
    matrix.Set(2, 2, 0.0);
    matrix.Set(0, 1, 2.0);
    matrix.Set(0, 2, 2.0);
    matrix.Set(1, 2, 2.0);

    const std::array<double, 3> eigenvalues = {-2, -2, 4};
    CheckCalculationSingleMatrix(matrix, eigenvalues, false, 1.0e-3);
}

TEST(EigenValueSolverSym3x3Test, TestTwoIdenticalEigenvalues3)
{
    auto matrix = MatrixKelvinRepr3::CreateEmpty();
    matrix.Set(2, 2, 1.0);

    const std::array<double, 3> eigenvalues = {0, 0, 1};
    CheckCalculationSingleMatrix(matrix, eigenvalues, false, 1.0e-3);
}

TEST(EigenValueSolverSym3x3Test, TestTwoIdenticalEigenvalues4)
{
    auto matrix = MatrixKelvinRepr3::CreateEmpty();
    for (int n = 0; n < 5; ++n)
    {
        for (int flat = 0; flat < 6; ++flat)
        {
            int i, j;
            IndexerKelvinRepr3::ToRowCol(flat, i, j);
            matrix.Set(i, j, static_cast<double>(n));
        }

        const std::array<double, 3> eigenvalues = {0, 0, 3.0 * n};
        CheckCalculationSingleMatrix(matrix, eigenvalues, false, 1.0e-3);
    }
}

TEST(EigenValueSolverSym3x3Test, TestThreeIdenticalEigenvalues2)
{
    const auto matrix = MatrixKelvinRepr3::CreateEmpty();

    const std::array<double, 3> eigenvalues = {0, 0, 0};
    CheckCalculationSingleMatrix(matrix, eigenvalues, false, 1.0e-3);
}

TEST(EigenValueSolverSym3x3Test, TestFunctionQ)
{
    std::array<double, 6> x = {1, 2, 3, 4, 5, 6};
    const double eval = EigenValueSolverSym3x3Utils::EvaluateQ(x);
    ASSERT_NEAR(eval, 2.0, eps);
}

TEST(EigenValueSolverSym3x3Test, Test1FunctionP)
{
    EigenValueSolverSym3x3Utils::FunctionP fie;
    ASSERT_FALSE(fie.DerivativeAlwaysZero(0));
    ASSERT_FALSE(fie.DerivativeAlwaysZero(1));
    ASSERT_FALSE(fie.DerivativeAlwaysZero(2));
    ASSERT_FALSE(fie.DerivativeAlwaysZero(3));
    ASSERT_FALSE(fie.DerivativeAlwaysZero(4));
    ASSERT_FALSE(fie.DerivativeAlwaysZero(5));

    auto matrix = MatrixKelvinRepr3::CreateEmpty();
    matrix.Set(0, 0, 3.0);
    matrix.Set(1, 1, 0.0);
    matrix.Set(2, 2, 3.0);
    matrix.Set(0, 1, 2.0);
    matrix.Set(0, 2, 4.0);
    matrix.Set(1, 2, 2.0);
    const double eval = fie.Evaluate(matrix.Vector());
    ASSERT_NEAR(eval, 3.0, eps);

    std::vector<double> delx(6, 0.1);
    RealFunctionCheckDerivative::Check(fie, matrix.Vector(), delx);
}

TEST(EigenValueSolverSym3x3Test, Test2FunctionP)
{
    EigenValueSolverSym3x3Utils::FunctionP fie;

    std::array<double, 6> pderiv;
    double pval;

    {
        auto matrix = MatrixKelvinRepr3::CreateEmpty();
        matrix.Set(0, 0, 1.0);

        pval = fie.Evaluate(matrix.Vector());
        ASSERT_NEAR(pval, 1.0 / 3.0, eps);
        fie.Derivative(matrix.Vector(), pderiv);
        ASSERT_NEAR(pderiv[0], 1.0 / 3.0, eps);
        ASSERT_NEAR(pderiv[1], -1.0 / 6.0, eps);
        ASSERT_NEAR(pderiv[2], -1.0 / 6.0, eps);
        ASSERT_NEAR(pderiv[3], 0.0, eps);
        ASSERT_NEAR(pderiv[4], 0.0, eps);
        ASSERT_NEAR(pderiv[5], 0.0, eps);
    }

    {
        auto matrix = MatrixKelvinRepr3::CreateEmpty();
        matrix.Set(1, 1, 1.0);

        pval = fie.Evaluate(matrix.Vector());
        ASSERT_NEAR(pval, 1.0 / 3.0, eps);
        fie.Derivative(matrix.Vector(), pderiv);
        ASSERT_NEAR(pderiv[1], 1.0 / 3.0, eps);
        ASSERT_NEAR(pderiv[0], -1.0 / 6.0, eps);
        ASSERT_NEAR(pderiv[2], -1.0 / 6.0, eps);
        ASSERT_NEAR(pderiv[3], 0.0, eps);
        ASSERT_NEAR(pderiv[4], 0.0, eps);
        ASSERT_NEAR(pderiv[5], 0.0, eps);
    }

    {
        auto matrix = MatrixKelvinRepr3::CreateEmpty();
        matrix.Set(0, 1, 1.0);

        pval = fie.Evaluate(matrix.Vector());
        ASSERT_NEAR(pval, std::sqrt(1.0 / 3.0), eps);
        fie.Derivative(matrix.Vector(), pderiv);
        ASSERT_NEAR(pderiv[0], 0.0, eps);
        ASSERT_NEAR(pderiv[1], 0.0, eps);
        ASSERT_NEAR(pderiv[2], 0.0, eps);
        ASSERT_NEAR(pderiv[3], 0.0, eps);
        ASSERT_NEAR(pderiv[4], 0.0, eps);
        ASSERT_NEAR(pderiv[5], 1.0 / std::sqrt(6), eps);
    }
}

TEST(EigenValueSolverSym3x3Test, TestFunctionBComp02)
{
    for (int n = 0; n < 3; ++n)
    {
        EigenValueSolverSym3x3Utils::ComponentAMinQI fie(n);

        ASSERT_FALSE(fie.DerivativeAlwaysZero(0));
        ASSERT_FALSE(fie.DerivativeAlwaysZero(1));
        ASSERT_FALSE(fie.DerivativeAlwaysZero(2));
        ASSERT_TRUE(fie.DerivativeAlwaysZero(3));
        ASSERT_TRUE(fie.DerivativeAlwaysZero(4));
        ASSERT_TRUE(fie.DerivativeAlwaysZero(5));

        std::array<double, 6> x = {1, 2, 3, 4, 5, 6};

        const double eval = fie.Evaluate(x);
        ASSERT_NEAR(eval, x[n] - 2, eps);

        std::vector<double> delx(6, 0.01);
        RealFunctionCheckDerivative::Check(fie, x, delx);
    }
}

TEST(EigenValueSolverSym3x3Test, TestFunctionBComp35)
{
    for (int n = 3; n < 6; ++n)
    {
        EigenValueSolverSym3x3Utils::ComponentAMinQI fie(n);

        ASSERT_TRUE(fie.DerivativeAlwaysZero(0));
        ASSERT_TRUE(fie.DerivativeAlwaysZero(1));
        ASSERT_TRUE(fie.DerivativeAlwaysZero(2));
        ASSERT_EQ(n != 3, fie.DerivativeAlwaysZero(3));
        ASSERT_EQ(n != 4, fie.DerivativeAlwaysZero(4));
        ASSERT_EQ(n != 5, fie.DerivativeAlwaysZero(5));

        std::array<double, 6> x = {1, 2, 3, 4, 5, 6};

        const double eval = fie.Evaluate(x);
        ASSERT_NEAR(eval, x[n], eps);

        std::vector<double> delx(6, 0.01);
        RealFunctionCheckDerivative::Check(fie, x, delx);
    }
}

TEST(EigenValueSolverSym3x3Test, TestFunctionDetB)
{
    EigenValueSolverSym3x3Utils::FunctionP pFie;
    EigenValueSolverSym3x3Utils::FunctionDetB fie(pFie);
    ASSERT_FALSE(fie.DerivativeAlwaysZero(0));
    ASSERT_FALSE(fie.DerivativeAlwaysZero(1));
    ASSERT_FALSE(fie.DerivativeAlwaysZero(2));
    ASSERT_FALSE(fie.DerivativeAlwaysZero(3));
    ASSERT_FALSE(fie.DerivativeAlwaysZero(4));
    ASSERT_FALSE(fie.DerivativeAlwaysZero(5));

    auto matrix = MatrixKelvinRepr3::CreateEmpty();
    matrix.Set(0, 0, 3.0);
    matrix.Set(1, 1, 0.0);
    matrix.Set(2, 2, 3.0);
    matrix.Set(0, 1, 2.0);
    matrix.Set(0, 2, 4.0);
    matrix.Set(1, 2, 2.0);
    const double eval = fie.Evaluate(matrix.Vector());
    ASSERT_NEAR(eval, 2.0, eps);

    std::vector<double> delx(6, 1.0e-3);
    RealFunctionCheckDerivative::CheckExtrapolation(fie, matrix.Vector(), delx);
}

TEST(EigenValueSolverSym3x3Test, TestFunctionP3Inv)
{
    EigenValueSolverSym3x3Utils::FunctionP3Inv fie;
    ;

    auto matrix = MatrixKelvinRepr3::CreateEmpty();
    matrix.Set(0, 0, 3.0);
    matrix.Set(1, 1, 0.0);
    matrix.Set(2, 2, 3.0);
    matrix.Set(0, 1, 2.0);
    matrix.Set(0, 2, 4.0);
    matrix.Set(1, 2, 2.0);
    const double eval = fie.Evaluate(matrix.Vector());
    ASSERT_NEAR(eval, 1.0 / 27.0, eps);

    std::vector<double> delx(6, 0.01);
    RealFunctionCheckDerivative::Check(fie, matrix.Vector(), delx);
}

TEST(EigenValueSolverSym3x3Test, TestDetAMinQI)
{
    EigenValueSolverSym3x3Utils::DetAMinQI fie;
    ;

    auto matrix = MatrixKelvinRepr3::CreateEmpty();
    matrix.Set(0, 0, 3.0);
    matrix.Set(1, 1, 0.0);
    matrix.Set(2, 2, 3.0);
    matrix.Set(0, 1, 2.0);
    matrix.Set(0, 2, 4.0);
    matrix.Set(1, 2, 2.0);
    const double eval = fie.Evaluate(matrix.Vector());
    ASSERT_NEAR(eval, 54, eps);

    std::vector<double> delx(6, 0.01);
    RealFunctionCheckDerivative::Check(fie, matrix.Vector(), delx);
}

TEST(EigenValueSolverSym3x3Test, TestDetAMinQI_2)
{
    EigenValueSolverSym3x3Utils::DetAMinQI fie;
    ;

    auto matrix = MatrixKelvinRepr3::CreateEmpty();
    matrix.Set(0, 0, 1.1);
    matrix.Set(1, 1, 2.3);
    matrix.Set(2, 2, 3.4);
    matrix.Set(0, 1, 4.53436);
    matrix.Set(0, 2, 5.7);
    matrix.Set(1, 2, 6.0);
    const double eval = fie.Evaluate(matrix.Vector());

    std::vector<double> delx(6, 0.1);
    RealFunctionCheckDerivative::Check(fie, matrix.Vector(), delx);
}

TEST(EigenValueSolverSym3x3Test, CheckDerivsFunctionP)
{
    std::array<double, 6> matrix;
    std::vector<double> delx(6, 0.01);
    EigenValueSolverSym3x3Utils::FunctionP fieP;
    std::srand(0);
    const int maxIter = 10;
    for (int iter = 0; iter < maxIter; ++iter)
    {
        for (int flat = 0; flat < 6; ++flat)
        {
            matrix[flat] = RandomNumber();
        }

        RealFunctionCheckDerivative::Check(fieP, matrix, delx);
    }
}

TEST(EigenValueSolverSym3x3Test, CheckDerivsFunctionP3Inv)
{
    std::array<double, 6> matrix;
    std::vector<double> delx(6, 0.01);
    EigenValueSolverSym3x3Utils::FunctionP3Inv fieP3Inv;
    std::srand(0);
    const int maxIter = 10;
    for (int iter = 0; iter < maxIter; ++iter)
    {
        for (int flat = 0; flat < 6; ++flat)
        {
            matrix[flat] = RandomNumber();
        }

        RealFunctionCheckDerivative::Check(fieP3Inv, matrix, delx);
    }
}

TEST(EigenValueSolverSym3x3Test, CheckDerivsComponentAMinQI)
{
    std::array<double, 6> matrix;
    std::vector<double> delx(6, 0.01);
    EigenValueSolverSym3x3Utils::FunctionP3Inv fieP3Inv;
    std::srand(0);
    const int maxIter = 10;
    for (int iter = 0; iter < maxIter; ++iter)
    {
        for (int flat = 0; flat < 6; ++flat)
        {
            matrix[flat] = RandomNumber();
        }

        for (int cmp = 0; cmp < 6; ++cmp)
        {
            EigenValueSolverSym3x3Utils::ComponentAMinQI fieComp(cmp);
            RealFunctionCheckDerivative::Check(fieComp, matrix, delx);
        }
    }
}

TEST(EigenValueSolverSym3x3Test, CheckDerivsDetAMinQI)
{
    std::array<double, 6> matrix;
    std::vector<double> delx(6, 0.01);
    EigenValueSolverSym3x3Utils::DetAMinQI fieDetAMinQI;
    std::srand(0);
    const int maxIter = 10;
    for (int iter = 0; iter < maxIter; ++iter)
    {
        for (int flat = 0; flat < 6; ++flat)
        {
            matrix[flat] = RandomNumber();
        }

        RealFunctionCheckDerivative::Check(fieDetAMinQI, matrix, delx);
    }
}

TEST(EigenValueSolverSym3x3Test, CheckDerivsFunctionDetB)
{
    std::array<double, 6> matrix;
    std::vector<double> delx(6, 1.0e-3);
    EigenValueSolverSym3x3Utils::FunctionP fieP;
    EigenValueSolverSym3x3Utils::FunctionDetB fieDetB(fieP);
    std::srand(0);
    const int maxIter = 10;
    for (int iter = 0; iter < maxIter; ++iter)
    {
        for (int flat = 0; flat < 6; ++flat)
        {
            matrix[flat] = RandomNumber();
        }

        RealFunctionCheckDerivative::CheckExtrapolation(fieDetB, matrix, delx);
    }
}

TEST(EigenValueSolverSym3x3Test, CheckDerivsEigenValueSolverSym3x3)
{
    std::array<double, 6> matrix;
    std::vector<double> delx(6, 0.01);
    EigenValueSolverSym3x3 solver;
    std::srand(0);
    const int maxIter = 10;
    for (int iter = 0; iter < maxIter; ++iter)
    {
        for (int flat = 0; flat < 6; ++flat)
        {
            matrix[flat] = RandomNumber();
        }

        RealFunctionCheckDerivative::Check(solver, matrix, delx);
    }
}
