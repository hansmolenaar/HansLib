#pragma once

#include "MultiVariableFunctionEvaluateComposeWithSingle.h"
#include "SingleVariablePolynomial.h"
#include <memory>

class MultiVariableFunctionEvaluateFlipped : public MultiVariableFunctionEvaluateComposeWithSingle
{
  public:
    MultiVariableFunctionEvaluateFlipped(std::shared_ptr<IMultiVariableFunctionEvaluate> fie)
        : MultiVariableFunctionEvaluateComposeWithSingle(
              std::shared_ptr<ISingleVariableRealValuedFunction>(std::make_shared<SingleVariablePolynomial>(
                  std::vector<std::tuple<double, int>>{std::make_tuple(-1.0, 1)})),
              fie)
    {
    }
};