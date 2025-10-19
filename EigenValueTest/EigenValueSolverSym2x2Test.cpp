#include <gtest/gtest.h>

#include "EigenValueSolverSym2x2.h"
#include "Defines.h"
#include "MatrixKelvinRepr2.h"
#include "MyAssert.h"
#include "IRealFunctionUtils.h"

namespace
{
   constexpr double eps = 1.0e-10;

   void CheckAllDerivatives(std::span<const double> x)
   {
      Utilities::MyAssert(x.size() == 3);
      std::vector<double> delx(x.size(), 0.01);

      EigenValueSolverSym2x2Utils::Det fieDet;
      CheckDerivatives(fieDet, x, delx);

      EigenValueSolverSym2x2 solver;
      CheckDerivatives(solver, x, delx);
   }
}

TEST(EigenValueSolverSym2x2Test, TestBasis)
{
   EigenValueSolverSym2x2 solver;
   ASSERT_EQ(2, solver.GetRangeDimension());
   ASSERT_EQ(3, solver.GetDomainDimension());
   ASSERT_TRUE(solver.HasDerivative());
}

TEST(EigenValueSolverSym2x2Test, Identity)
{
   auto mat = MatrixKelvinRepr2::Create();
   mat.Set(0,0, 1.0);
   mat.Set(1,1, 1.0);

   std::vector<double> eigenvalues(GeomDim2);
   EigenValueSolverSym2x2::CalculateEigenvalues2x2(mat, eigenvalues);

   ASSERT_NEAR(eigenvalues[0], 1, eps);
   ASSERT_NEAR(eigenvalues[1], 1, eps);

   str::fill(eigenvalues, 0.0);
   EigenValueSolverSym2x2 solver;
   solver.Evaluate(mat.Vector(), eigenvalues);
   ASSERT_NEAR(eigenvalues[0], 1, eps);
   ASSERT_NEAR(eigenvalues[1], 1, eps);

   CheckAllDerivatives(mat.Vector());
}

TEST(EigenValueSolverSym2x2Test, Diagonal)
{
   auto mat = MatrixKelvinRepr2::Create();
   mat.Set(0,0, 1.0);
   mat.Set(1,1, 2.0);

   std::vector<double> eigenvalues{0,0};
   EigenValueSolverSym2x2 solver;
   solver.Evaluate(mat.Vector(), eigenvalues);
   ASSERT_NEAR(eigenvalues[0], 1, eps);
   ASSERT_NEAR(eigenvalues[1], 2, eps);

   CheckAllDerivatives(mat.Vector());
}


TEST(EigenValueSolverSym2x2Test, General)
{
   auto mat = MatrixKelvinRepr2::Create();
   mat.Set(0,0, 6.0);
   mat.Set(1,1, 4.0);
   mat.Set(0,1, 3.0);

   std::vector<double> eigenvalues{0,0};
   EigenValueSolverSym2x2 solver;
   solver.Evaluate(mat.Vector(), eigenvalues);
   ASSERT_NEAR(eigenvalues[0], 5 - std::sqrt(10.0), eps);
   ASSERT_NEAR(eigenvalues[1], 5 + std::sqrt(10.0), eps);

   CheckAllDerivatives(mat.Vector());
}
