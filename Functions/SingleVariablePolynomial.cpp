#include "Functions/SingleVariablePolynomial.h"


void SingleVariablePolynomial::Add(double coef, SingleVariableMonomial monomial)
{
	m_polynomial.push_back(std::make_pair(coef, monomial));
}

double SingleVariablePolynomial::Evaluate(double x)const
{
	double result = 0;
	for (auto& term : m_polynomial)
	{
		result += term.first * term.second.Evaluate(x);
	}
	return result;
}

double SingleVariablePolynomial::Derivative(double x)const
{
	double result = 0;
	for (auto& term : m_polynomial)
	{
		result += term.first * term.second.Derivative(x);
	}
	return result;
}


bool SingleVariablePolynomial::HasDerivative() const
{
	return true;
}


bool SingleVariablePolynomial::IsNonConstant() const
{
	for (auto& term : m_polynomial)
	{
		if (term.second.IsNonConstant())
		{
			return true;
		}
	}
	return false;
}