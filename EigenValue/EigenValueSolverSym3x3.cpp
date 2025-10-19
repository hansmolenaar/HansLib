#include "EigenValueSolverSym3x3.h"

#include "MyAssert.h"
#include "MathConstants.h"
#include "IMatrixSymmetric.h"
#include "IMatrixSquareUtils.h"
#include "MatrixKelvinRepr3.h"
#include "IMatrixKelvinReprUtils.h"
#include "IRealFunctionUtils.h"
#include "MultiVariableVectorComponentFunction.h"
#include "MultiVariableRealValuedFunction.h"
#include "MultiVariableVectorValuedFunction.h"
#include "MultiVariableMultiplicativeInverseFunction.h"
#include "MultiVariableProductFunction.h"
#include "MatrixKelvinRepr3.h"
#include "IMatrixUtils.h"
#include "MatrixDense.h"
#include "Defines.h"


#include <numeric>
#include <algorithm>
#include <cmath>

namespace {
   constexpr std::array<double, 3> c_angles = { 0.0, 2 * MathConstants::PI / 3, 4 * MathConstants::PI / 3 };
}


// !!!!!!!!!!!!!!!!!!! FunctionP3Inv !!!!!!!!!!!!!!!!!!!!!


int EigenValueSolverSym3x3Utils::FunctionP3Inv::GetDomainDimension() const { return 6; }

double EigenValueSolverSym3x3Utils::FunctionP3Inv::Evaluate(std::span<const double>x)const
{
   const double pval = m_p.Evaluate(x);
   const double pval3 = pval * pval*pval;
   Utilities::MyAssert(pval3 != 0.0, "FunctionP3Inv::Evaluate try to divide by 0");
   return 1 / pval3;
}


void EigenValueSolverSym3x3Utils::FunctionP3Inv::Derivative(std::span<const double>x, std::span< double> dfdx)const
{
   const double pval = m_p.Evaluate(x);
   const double pval4 = pval * pval*pval*pval;
   Utilities::MyAssert(pval4 != 0.0, "FunctionP3Inv::Derivative try to divide by 0");
   const double factor = -3.0 / pval4;

   m_p.Derivative(x, dfdx);
   for (int n = 0; n < 6; ++n)
   {
      dfdx[n] *= factor;
   }
}

bool EigenValueSolverSym3x3Utils::FunctionP3Inv::DerivativeAlwaysZero(int var) const
{
   return false;
}

bool EigenValueSolverSym3x3Utils::FunctionP3Inv::HasDerivative() const
{
   return true;
}

// !!!!!!!!!!!!!!!!!!! DetAMinQI !!!!!!!!!!!!!!!!!!!!!


int EigenValueSolverSym3x3Utils::DetAMinQI::GetDomainDimension() const
{
   return 6;
}


bool EigenValueSolverSym3x3Utils::DetAMinQI::HasDerivative() const
{
   return true;
}

double EigenValueSolverSym3x3Utils::DetAMinQI::Evaluate(std::span<const double>x)const
{
   std::array<double, 6> bmat;
   for (int n = 0; n < 6; ++n)
   {
      bmat[n] = ComponentAMinQI(n).Evaluate(x);
   }
   const double result = bmat[0] * bmat[1] * bmat[2] +
      MathConstants::SQRT1_2*bmat[3] * bmat[4] * bmat[5] -
      0.5*(bmat[0] * bmat[3] * bmat[3] + bmat[1] * bmat[4] * bmat[4] + bmat[2] * bmat[5] * bmat[5]);
   return result;
}

void EigenValueSolverSym3x3Utils::DetAMinQI::Derivative(std::span<const double>x, std::span< double> dfdx)const
{
   std::fill_n(dfdx.data(), 6, 0.0);

   std::array<double, 6> bxxxDeriv;
   std::array<ComponentAMinQI, 6> bcomps = { ComponentAMinQI(0), ComponentAMinQI(1), ComponentAMinQI(2),ComponentAMinQI(3),ComponentAMinQI(4),ComponentAMinQI(5) };
   auto b012 = MultiVariableProductFunction({ &bcomps[0],&bcomps[1], &bcomps[2] });
   auto b345 = MultiVariableProductFunction({ &bcomps[3],&bcomps[4], &bcomps[5] });
   auto b033 = MultiVariableProductFunction({ &bcomps[0],&bcomps[3], &bcomps[3] });
   auto b144 = MultiVariableProductFunction({ &bcomps[1],&bcomps[4], &bcomps[4] });
   auto b255 = MultiVariableProductFunction({ &bcomps[2],&bcomps[5], &bcomps[5] });

   b012.Derivative(x, bxxxDeriv);
   for (int n = 0; n < 6; ++n)
   {
      dfdx[n] += bxxxDeriv[n];
   }

   b345.Derivative(x, bxxxDeriv);
   for (int n = 0; n < 6; ++n)
   {
      dfdx[n] += MathConstants::SQRT1_2*  bxxxDeriv[n];
   }

   b033.Derivative(x, bxxxDeriv);
   for (int n = 0; n < 6; ++n)
   {
      dfdx[n] -= 0.5*  bxxxDeriv[n];
   }

   b144.Derivative(x, bxxxDeriv);
   for (int n = 0; n < 6; ++n)
   {
      dfdx[n] -= 0.5*  bxxxDeriv[n];
   }

   b255.Derivative(x, bxxxDeriv);
   for (int n = 0; n < 6; ++n)
   {
      dfdx[n] -= 0.5*  bxxxDeriv[n];
   }
}

bool EigenValueSolverSym3x3Utils::DetAMinQI::DerivativeAlwaysZero(int var) const
{
   return false;
}




// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

// Roots of:  x^3 -3x - detB = 0




int EigenValueSolverSym3x3Utils::AuxilaryEquationRoots::GetRangeDimension() const
{
   return 3;
}
int EigenValueSolverSym3x3Utils::AuxilaryEquationRoots::GetDomainDimension() const
{
   return 1;
}


bool EigenValueSolverSym3x3Utils::AuxilaryEquationRoots::HasDerivative() const
{
   return true;
}

bool EigenValueSolverSym3x3Utils::AuxilaryEquationRoots::DerivativeAlwaysZero(int eqn, int var) const
{
   return false;
}
void EigenValueSolverSym3x3Utils::AuxilaryEquationRoots::Evaluate(std::span<const double>x, std::span< double> y) const
{
   Utilities::MyAssert(x.size() == 1);
   const double detB = ClipDetB(x[0]);
   const double theta = std::acos(detB / 2) / 3;

   str::transform(c_angles, y.begin(), [theta](auto x){ return 2*std::cos(theta + x);});
}
void EigenValueSolverSym3x3Utils::AuxilaryEquationRoots::Derivative(std::span<const double> x, IMatrix& derivs) const
{
   const double detB = ClipDetB(x[0]);

   const double theta = std::acos(detB / 2) / 3;


   const double arg = 1 - 0.25*detB * detB;
   Utilities::MyAssert(arg != 0.0, "AuxilaryEquationRoots::Derivative try to divide by 0");
   const double dThetadDetB = -1 / std::sqrt(arg) / 6;

   Clear(derivs);
   for (int n = 0; n < 3; ++n)
   {
      derivs(n, 0) = -2 * std::sin(theta + c_angles[n])*dThetadDetB;
   }
}

// !!!!!!!!!!!!! FunctionQ !!!!!!!!!!!!!!!!!!!!!!!!

double EigenValueSolverSym3x3Utils::EvaluateQ(std::span<const double>x)
{
   Utilities::MyAssert(x.size() == 6);
   return (x[0] + x[1] + x[2]) / 3;

}


double EigenValueSolverSym3x3Utils::ClipDetB(double detB)
{
   // Accept some noise in detB calculation
   Utilities::MyAssert(std::abs(detB) - 2 < 1.0e-10);
   detB = std::min(detB, 2.0);
   detB = std::max(detB, -2.0);
   Utilities::MyAssert(std::abs(detB) <= 2.0);
   return detB;
}

// !!!!!!!!!!!!! FunctionP !!!!!!!!!!!!!!!!!!!!!!!!


EigenValueSolverSym3x3Utils::FunctionP::FunctionP()
{
}

int EigenValueSolverSym3x3Utils::FunctionP::GetDomainDimension() const
{
   return 6;
}


bool EigenValueSolverSym3x3Utils::FunctionP::HasDerivative() const
{
   return true;
}

double EigenValueSolverSym3x3Utils::FunctionP::Evaluate(std::span<const double>x)const
{
   Utilities::MyAssert(x.size() == 6);
   double qval = EvaluateQ(x);
   double sum = 0;
   for (int n = 0; n < 6; ++n)
   {
      double term = x[n] - (n < 3 ? qval : 0);
      sum += term * term;
   }
   return std::sqrt(sum / 6);
}

void EigenValueSolverSym3x3Utils::FunctionP::Derivative(std::span<const double>x, std::span< double> dfdx)const
{
   Utilities::MyAssert(x.size() == 6);
   Utilities::MyAssert(dfdx.size() == 6);
   const double fie = Evaluate(x);
   Utilities::MyAssert(fie != 0.0, "FunctionP::Derivative try to divide by 0");
   const double fac = 1 / (2 * fie) / 6.0;
   const double qval = EvaluateQ(x);
   std::fill_n(dfdx.begin(), 6, 0.0);
   for (int n = 0; n < 3; ++n)
   {
      const double term = x[n] - qval;
      const double f2t = fac * 2 * term;
      dfdx[n] += f2t;
      const double f2to3 = f2t / 3.0;
      for (int nn = 0; nn < 3; ++nn)
      {
         dfdx[nn] -= f2to3;
      }
   }
   for (int n = 3; n < 6; ++n)
   {
      const double term = x[n];
      dfdx[n] = fac * 2 * term;
   }
}
bool EigenValueSolverSym3x3Utils::FunctionP::DerivativeAlwaysZero(int var) const
{
   return false;
}


// !!!!!!!!!!!!! ComponentAMinQI !!!!!!!!!!!!!!!!!!!!!!!!


EigenValueSolverSym3x3Utils::ComponentAMinQI::ComponentAMinQI(int n) : m_n(n)
{
   Utilities::MyAssert(n >= 0 && n < 6);
}

int EigenValueSolverSym3x3Utils::ComponentAMinQI::GetDomainDimension() const
{
   return 6;
}


bool EigenValueSolverSym3x3Utils::ComponentAMinQI::HasDerivative() const
{
   return true;
}

double EigenValueSolverSym3x3Utils::ComponentAMinQI::Evaluate(std::span<const double>x)const
{
   Utilities::MyAssert(x.size() == 6);
   double result = x[m_n];
   if (m_n < 3)
   {
      result -= EvaluateQ(x);
   }
   return result;
}

bool EigenValueSolverSym3x3Utils::ComponentAMinQI::DerivativeAlwaysZero(int var) const
{
   if (m_n < 3)
   {
      return var >= 3;
   }
   else
   {
      return var != m_n;
   }
}


void EigenValueSolverSym3x3Utils::ComponentAMinQI::Derivative(std::span<const double>x, std::span< double> dfdx) const
{
   std::fill_n(dfdx.data(), 6, 0.0);
   dfdx[m_n] = 1.0;
   if (m_n < 3)
   {
      for (int n = 0; n < 3; ++n)
      {
         dfdx[n] -= 1.0 / 3.0;
      }
   }
}

// !!!!!!!!!!!!! FunctionDetB !!!!!!!!!!!!!!!!!!!!!!!!


EigenValueSolverSym3x3Utils::FunctionDetB::FunctionDetB(const IMultiVariableRealValuedFunction& p) :
   m_p(p)
{
   Utilities::MyAssert(m_p.GetDomainDimension() == 6);
}

int EigenValueSolverSym3x3Utils::FunctionDetB::GetDomainDimension() const
{
   return 6;
}


bool EigenValueSolverSym3x3Utils::FunctionDetB::HasDerivative() const
{
   return true;
}

double EigenValueSolverSym3x3Utils::FunctionDetB::Evaluate(std::span<const double>x)const
{
   FunctionP3Inv p3inv;
   DetAMinQI detAMinQI;
   MultiVariableProductFunction prod({ &p3inv, &detAMinQI });
   return prod.Evaluate(x);
}

bool EigenValueSolverSym3x3Utils::FunctionDetB::DerivativeAlwaysZero(int var) const
{
   return false;
}


void EigenValueSolverSym3x3Utils::FunctionDetB::Derivative(std::span<const double>x, std::span< double> dfdx)const
{
   FunctionP3Inv p3inv;
   DetAMinQI detAMinQI;
   MultiVariableProductFunction prod({ &p3inv, &detAMinQI });
   prod.Derivative(x, dfdx);
}


// !!!!!!!!!!!!!!!!!!!!!! REAL THING


int EigenValueSolverSym3x3::GetRangeDimension() const
{
   return 3;
}
int EigenValueSolverSym3x3::GetDomainDimension() const
{
   return 6;
}
bool EigenValueSolverSym3x3::DerivativeAlwaysZero(int eqn, int var) const
{
   return false;
}


bool EigenValueSolverSym3x3::HasDerivative() const
{
   return true;
}

void EigenValueSolverSym3x3::Evaluate(std::span<const double> x, std::span< double> eigenValues) const
{
   const double c_eps = 1.0e-10;
   Utilities::MyAssert(x.size() == 6);
   const double q = (x[0] + x[1] + x[2]) / 3;
   EigenValueSolverSym3x3Utils::FunctionP pFie;
   const double p = pFie.Evaluate(x);

   if (std::abs(p) > c_eps)
   {
      EigenValueSolverSym3x3Utils::FunctionDetB detBFunction(pFie);
      const double detB = detBFunction.Evaluate(x);

      EigenValueSolverSym3x3Utils::AuxilaryEquationRoots auxSolver;
      std::array<double, 3> roots;
      auxSolver.Evaluate(std::span<const double>(&detB, 1), roots);

      for (int n = 0; n < 3; ++n)
      {
         eigenValues[n] = p * roots[n] + q;
      }
   }
   else
   {
      // Simple problem, 3 identical eigenvalues
      std::fill_n(eigenValues.begin(), 3, q);
   }
}

void EigenValueSolverSym3x3::Derivative(std::span<const double>x, IMatrix& dfdx) const
{
   const double c_eps = 1.0e-10;
   Utilities::MyAssert(x.size() == 6);
   Utilities::MyAssert(dfdx.GetRowDimension() == 3);
   Utilities::MyAssert(dfdx.GetColDimension() == 6);

   EigenValueSolverSym3x3Utils::FunctionP pFie;
   const double pEval = pFie.Evaluate(x);


   Clear(dfdx);

   if (std::abs(pEval) > c_eps)
   {
      std::array<double, 6> pDerivs;
      pFie.Derivative(x, pDerivs);

      EigenValueSolverSym3x3Utils::FunctionDetB detBFunction(pFie);
      const double detB = detBFunction.Evaluate(x);
      std::array<double, 6> derivDetB;
      detBFunction.Derivative(x, derivDetB);

      EigenValueSolverSym3x3Utils::AuxilaryEquationRoots auxSolver;
      std::array<double, 3> roots;
      auxSolver.Evaluate(std::span<const double>(&detB, 1), roots);
      MatrixDense rootsDerivs(3, 1);
      auxSolver.Derivative(std::span<const double>(&detB, 1), rootsDerivs);

      //eigenValues[n] = p * roots[n] + q;
      for (int r = 0; r < 3; ++r)
      {
         // Constribution from q
         for (int n = 0; n < 3; ++n)
         {
            dfdx(r, n) += 1.0 / 3.0;
         }

         // Constribution from p*root
         for (int n = 0; n < 6; ++n)
         {
            dfdx(r, n) += pDerivs[n] * roots[r];
            dfdx(r, n) += pEval * rootsDerivs(r, 0) * derivDetB[n];
         }
      }
   }
   else
   {
      // Simple problem, 3 identical eigenvalues. But are the derivatives also simple?

      for (int n0 = 0; n0 < 3; ++n0)
      {
         for (int n1 = 0; n1 < 3; ++n1)
         {
            dfdx(n0, n1) = 1.0 / 3.0;
         }
      }
   }
}

void EigenValueSolverSym3x3::CalculateEigenvalues3x3(const MatrixKelvinRepr3& matrix, std::span<double> eigenValues)
{
   EigenValueSolverSym3x3().Evaluate(matrix.Vector(), eigenValues);
}
