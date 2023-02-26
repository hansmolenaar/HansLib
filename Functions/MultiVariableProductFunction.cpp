#include "MultiVariableProductFunction.h"
#include "MyAssert.h"

#include <algorithm>

MultiVariableProductFunction::MultiVariableProductFunction(std::initializer_list< const IMultiVariableRealValuedFunction *> functions) :
	m_functions(functions.begin(), functions.end())
{
	m_dim = (*m_functions.begin())->GetDomainDimension();
	for (auto f : m_functions)
	{
		Utilities::MyAssert(f->GetDomainDimension() == m_dim);
	}
}

int MultiVariableProductFunction::GetDomainDimension() const
{
	return m_dim;
}


bool MultiVariableProductFunction::HasDerivative() const
{
	return std::all_of(m_functions.begin(), m_functions.end(), [=](const IMultiVariableRealValuedFunction* fie) { return fie->HasDerivative(); });
}


bool MultiVariableProductFunction::DerivativeAlwaysZero(int var) const
{
	return std::all_of(m_functions.begin(), m_functions.end(), [=](const IMultiVariableRealValuedFunction* fie) { return fie->DerivativeAlwaysZero(var); });
}

double MultiVariableProductFunction::Evaluate(std::span<const double>x) const
{
	Utilities::MyAssert(x.size() == m_dim);
	double result = 1.0;
	for (const auto fie : m_functions)
	{
		result *= fie->Evaluate(x);
	}

	return result;
}

void MultiVariableProductFunction::Derivative(std::span<const double>x, std::span< double> dfdx)const
{
	Utilities::MyAssert(x.size() == m_dim);
	Utilities::MyAssert(dfdx.size() == m_dim);

	std::vector<double> evals(m_functions.size());
	for (auto n = 0; n < m_functions.size(); ++n)
	{
		evals[n] = m_functions[n]->Evaluate(x);
	}

	std::vector<double> deriv(m_dim);

	std::fill_n(dfdx.data(), m_dim, 0.0);
	for (int f = 0; f < m_functions.size(); ++f)
	{
		m_functions[f]->Derivative(x, deriv);
		double term = 1.0;
		for (int n = 0; n < m_functions.size(); ++n)
		{
			if (n != f)
			{
				term *= evals[n];
			}
		}
		for (auto var = 0; var < m_dim; ++var)
		{
			if (!DerivativeAlwaysZero(var))
			{
				dfdx[var] += term * deriv[var];
			}
		}
	}
}