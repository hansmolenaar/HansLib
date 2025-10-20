#pragma once

#include "IMultiVariableFunctionEvaluate.h"
#include "ISingleVariableRealValuedFunction.h"

class MultiVariableFunctionEvaluateComposeWithSingle : public IMultiVariableFunctionEvaluate
{
  public:
    MultiVariableFunctionEvaluateComposeWithSingle(std::shared_ptr<ISingleVariableRealValuedFunction> transform,
                                                   std::shared_ptr<IMultiVariableFunctionEvaluate> function);

    int getDimension() const override;
    double operator()(std::span<const double> x) const override;

  private:
    std::shared_ptr<IMultiVariableFunctionEvaluate> m_function;
    std::shared_ptr<ISingleVariableRealValuedFunction> m_transform;
};