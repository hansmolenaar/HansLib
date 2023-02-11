#pragma once

#include "IMultiVariableRealValuedFunction.h"
#include "IMultiVariableFunctionEvaluate.h"
#include <memory>

class MultiVariableRealValuedFunctionNoDerivatives : public IMultiVariableRealValuedFunction
{
public:
   MultiVariableRealValuedFunctionNoDerivatives(int dim, std::unique_ptr<IMultiVariableFunctionEvaluate>&& function);
   int GetDomainDimension() const override { return m_dim; }
   double Evaluate(std::span<const double>x)const override;
   void Derivative(std::span<const double>x, std::span< double> dfdx)const override { throw MyException("Not implemented"); }
   virtual bool DerivativeAlwaysZero(int var) const override { throw MyException("Not implemented"); }
   bool HasDerivative() const override { return false; }

private:
   int m_dim;
   std::unique_ptr<IMultiVariableFunctionEvaluate> m_function;
};