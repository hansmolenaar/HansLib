#include "SingleVariableFunctionOnInterval.h"
#include "SingleVariablePolynomial.h"

SingleVariableFunctionOnInterval::SingleVariableFunctionOnInterval(
    std::shared_ptr<ISingleVariableRealValuedFunction> fie, const Interval<double> &domain)
    : m_function(fie), m_domain(domain)
{
}

const Interval<double> &SingleVariableFunctionOnInterval::getDomain() const
{
    return m_domain;
}

double SingleVariableFunctionOnInterval::Evaluate(double x) const
{
    if (!m_domain.contains(x))
    {
        throw MyException("SingleVariableFunctionOnInterval::Evaluate outside range");
    }
    return m_function->Evaluate(x);
}

double SingleVariableFunctionOnInterval::Derivative(double x) const
{
    if (!m_domain.contains(x))
    {
        throw MyException("SingleVariableFunctionOnInterval::Derivative outside range");
    }
    return m_function->Derivative(x);
}

bool SingleVariableFunctionOnInterval::IsNonConstant() const
{
    return m_function->IsNonConstant();
}

bool SingleVariableFunctionOnInterval::HasDerivative() const
{
    return m_function->HasDerivative();
}

std::shared_ptr<SingleVariableFunctionOnInterval> SingleVariableFunctionOnInterval::CreateLinearOnInterval(
    const Interval<double> intv, double valL, double valR)
{
    const double factor = (valR - valL) / intv.getMeasure();
    auto poly = SingleVariablePolynomial::Create(std::vector<std::tuple<double, int>>{
        std::make_tuple(factor, 1), std::make_tuple(valL - intv.getLower() * factor, 0)});
    std::shared_ptr<ISingleVariableRealValuedFunction> fun = std::make_shared<SingleVariablePolynomial>(poly);
    return std::make_shared<SingleVariableFunctionOnInterval>(fun, intv);
}