#include "MultiVariableRealValuedFunctionNoDerivatives.h"

double MultiVariableRealValuedFunctionNoDerivatives::Evaluate(std::span<const double> x) const
{
    if (x.size() != m_dim)
        throw MyException("FunctionNoDerivativesGiven::Evaluate: input dimension incorrect");
    return (*m_function)(x);
}

MultiVariableRealValuedFunctionNoDerivatives::MultiVariableRealValuedFunctionNoDerivatives(
    int dim, std::shared_ptr<IMultiVariableFunctionEvaluate> function)
    : m_dim(dim), m_function(function)
{
}