#include "Utilities/EigenValueSolverSym2x2.h"
#include "KelvinRepr/MatrixKelvinRepr2.h"
#include "MatrixVector/IMatrixUtils.h"
#include "Utilities/Assert.h"

#if 0
#include "Utilities/MathConstants.h"
#include "MatrixVector/IMatrixSymmetric.h"
#include "MatrixVector/IMatrixSquareUtils.h"
#include "MatrixVector/MatrixKelvinRepr3.h"
#include "MatrixVector/IMatrixKelvinReprUtils.h"
#include "Functions/IRealFunctionUtils.h"
#include "Functions/MultiVariableVectorComponentFunction.h"
#include "Functions/MultiVariableRealValuedFunction.h"
#include "Functions/MultiVariableVectorValuedFunction.h"
#include "Functions/MultiVariableMultiplicativeInverseFunction.h"
#include "Functions/MultiVariableProductFunction.h"


#include "MatrixVector/MatrixDense.h"
#endif

#include <numeric>
#include <algorithm>




int EigenValueSolverSym2x2Utils::Det::GetDomainDimension() const
{
   return 3;
}

bool EigenValueSolverSym2x2Utils::Det::HasDerivative() const
{
   return true;
}

bool EigenValueSolverSym2x2Utils::Det::DerivativeAlwaysZero(int) const
{
   return false;
}

double EigenValueSolverSym2x2Utils::Det::Evaluate(std::span<const double>x) const
{
   Utilities::Assert(x.size() == 3);
   const double a = x[0];
   const double b = x[1];

   const double det = (a - b)*(a - b) + 2 * x[2] * x[2];
   return det;
}

void EigenValueSolverSym2x2Utils::Det::Derivative(std::span<const double>x, std::span< double> dfdx) const
{
   Utilities::Assert(x.size() == 3);
   Utilities::Assert(dfdx.size() == 3);
   dfdx[0] = 2 * x[0] - 2 * x[1];
   dfdx[1] = 2 * x[1] - 2 * x[0];
   dfdx[2] = 4 * x[2];
}

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

int EigenValueSolverSym2x2::GetRangeDimension() const
{
   return 2;
}
int EigenValueSolverSym2x2::GetDomainDimension() const
{
   return 3;
}
bool EigenValueSolverSym2x2::DerivativeAlwaysZero(int eqn, int var) const
{
   return false;
}

bool EigenValueSolverSym2x2::HasDerivative() const
{
   return true;
}

// ( a c )
// ( c b )
void EigenValueSolverSym2x2::Evaluate(std::span<const double> x, std::span< double> eigenValues) const
{
   Utilities::Assert(x.size() == 3);
   Utilities::Assert(eigenValues.size() == 2);

   const double a = x[0];
   const double b = x[1];

   const double det = EigenValueSolverSym2x2Utils::Det().Evaluate(x);
   const double sqrtDet = std::sqrt(det);

   eigenValues[0] = 0.5*(a + b - sqrtDet);
   eigenValues[1] = 0.5*(a + b + sqrtDet);
}

void EigenValueSolverSym2x2::Derivative(std::span<const double>x, IMatrix& dfdx) const
{
   Utilities::Assert(x.size() == 3);
   Utilities::Assert(dfdx.GetRowDimension() == 2);
   Utilities::Assert(dfdx.GetColDimension() == 3);

   const double det = EigenValueSolverSym2x2Utils::Det().Evaluate(x);
   const double sqrtDet = std::sqrt(det);
   Utilities::Assert(sqrtDet != 0.0); // TODO better test
   const double sqrtDetDeriv = 0.5 / sqrtDet;

   std::array<double, 3> detDeriv;
   EigenValueSolverSym2x2Utils::Det().Derivative(x, detDeriv);

   for (int c = 0; c < 3; ++c)
   {
      dfdx(0, c) = -sqrtDetDeriv * detDeriv[c];
      dfdx(1, c) = sqrtDetDeriv * detDeriv[c];
   }

   for (int n = 0; n < 2; ++n)
   {
      dfdx(n, 0) += 1;
      dfdx(n, 1) += 1;
   }

   dfdx *= 0.5;
}

void EigenValueSolverSym2x2::CalculateEigenvalues2x2(const MatrixKelvinRepr2& matrix, std::span<double> eigenValues)
{
   EigenValueSolverSym2x2().Evaluate(matrix.Vector(), eigenValues);
}
