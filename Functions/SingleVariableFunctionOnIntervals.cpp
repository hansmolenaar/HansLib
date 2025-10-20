#include "SingleVariableFunctionOnIntervals.h"
#include <cmath>

SingleVariableFunctionOnIntervals::SingleVariableFunctionOnIntervals(
    std::vector<std::shared_ptr<SingleVariableFunctionOnInterval>> fies, double defaultOutsideIntervals)
    : m_intervalFunctions(std::move(fies)), m_default(defaultOutsideIntervals)
{
}

double SingleVariableFunctionOnIntervals::Evaluate(double x) const
{
    for (const auto &fie : m_intervalFunctions)
    {
        if (fie->getDomain().contains(x))
        {
            return fie->Evaluate(x);
        }
    }
    if (std::isnan(m_default))
    {
        throw MyException("SingleVariableFunctionOnIntervals::evaluate no default");
    }
    return m_default;
}

double SingleVariableFunctionOnIntervals::Derivative(double x) const
{
    throw MyException("SingleVariableFunctionOnIntervals::Derivative not implemented");
}

bool SingleVariableFunctionOnIntervals::IsNonConstant() const
{
    return false;
}

bool SingleVariableFunctionOnIntervals::HasDerivative() const
{
    return false;
}