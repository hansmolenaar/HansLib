#include "MultiVariableFunctionEvaluateComposeWithSingle.h"

MultiVariableFunctionEvaluateComposeWithSingle::MultiVariableFunctionEvaluateComposeWithSingle(
    std::shared_ptr<ISingleVariableRealValuedFunction> transform,
    std::shared_ptr<IMultiVariableFunctionEvaluate> function)
    : m_function(function), m_transform(transform)
{
}

int MultiVariableFunctionEvaluateComposeWithSingle::getDimension() const
{
    return m_function->getDimension();
}

double MultiVariableFunctionEvaluateComposeWithSingle::operator()(std::span<const double> x) const
{
    const double val = (*m_function)(x);
    return (*m_transform)(val);
}