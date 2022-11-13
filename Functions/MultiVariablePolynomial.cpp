#include "Functions/MultiVariablePolynomial.h"

#include "Utilities/Assert.h"
#include "MatrixVector/IMatrix.h"
#include "MatrixVector/IMatrixUtils.h"
#include "Functions/IRealFunctionUtils.h"

#include <algorithm>
#include <numeric>
#include <ranges>

MultiVariablePolynomial::MultiVariablePolynomial(int dim) :
	m_dim(dim)
{
	Utilities::Assert(dim > 0);
}


void MultiVariablePolynomial::Add(double coef, std::span<const int> monomial)
{
	Utilities::Assert(monomial.size() == m_dim);
	m_terms.push_back(std::make_pair(coef, MultiVariableMonomial(monomial)));
}

void MultiVariablePolynomial::Add(double coef, std::initializer_list< int> monomial)
{
	Utilities::Assert(monomial.size() == m_dim);
	m_terms.push_back(std::make_pair(coef, MultiVariableMonomial(monomial)));
}

int MultiVariablePolynomial::GetDomainDimension() const
{
	return m_dim;
}


bool MultiVariablePolynomial::HasDerivative() const
{
	return true;
}

bool MultiVariablePolynomial::DerivativeAlwaysZero(int var) const
{
	return std::all_of(m_terms.begin(), m_terms.end(), [&](const auto& term) {return term.second.DerivativeAlwaysZero(var); });
}

double MultiVariablePolynomial::Evaluate(std::span<const double>x)const
{
	Utilities::Assert(x.size() == GetDomainDimension());
	double result = 0;
	for (const auto& term : m_terms)
	{
		const double eval = term.second.MultiVariableMonomial::Evaluate(x);
		result += term.first * eval;
	}
	return result;
	//return std::ranges::accumulate(m_terms | std::ranges::view::transform([&](auto& term) {const double eval = term.second.MultiVariableMonomial::Evaluate(x); return term.first*eval; }), 0.0);
}

void MultiVariablePolynomial::Derivative(std::span<const double> x, std::span<double> dfdx)const
{
	const int dim = GetDomainDimension();

	Utilities::Assert(x.size() == GetDomainDimension());
	Utilities::Assert(dfdx.size() == dim);


	std::fill(dfdx.begin(), dfdx.end(), 0.0);

	std::vector<double> deriv(dim);

	for (auto& term : m_terms)
	{
		::Derivative(term.second, x, deriv);
		for (int v = 0; v < dim; ++v)
		{
			dfdx[v] += term.first*deriv[v];
		}
	}
}