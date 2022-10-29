#pragma once

#include <array>
#include <span>
#include "Functions/MultiVariableRealValuedFunction.h"

class MatrixKelvinRepr2;
class IRealFunction;

class EigenValueSolverSym2x2 : public IRealFunction
{
public:
   int GetRangeDimension() const override;
   int GetDomainDimension() const override;
   bool DerivativeAlwaysZero(int eqn, int var) const override;
   void Evaluate(std::span<const double>x, std::span< double> y) const override;
   void Derivative(std::span<const double>x, IMatrix& dfdx) const override;
   bool HasDerivative() const override;

   static void CalculateEigenvalues2x2(const MatrixKelvinRepr2&, std::span<double>);
};


namespace EigenValueSolverSym2x2Utils
{

   class Det :public IMultiVariableRealValuedFunction
   {
   public:
      int GetDomainDimension() const override;
      double Evaluate(std::span<const double>x)const override;
      void Derivative(std::span<const double>x, std::span< double> dfdx)const override;
      bool DerivativeAlwaysZero(int var) const override;
      bool HasDerivative() const override;

      using IMultiVariableRealValuedFunction::DerivativeAlwaysZero;
      using IMultiVariableRealValuedFunction::Derivative;
   };



};
