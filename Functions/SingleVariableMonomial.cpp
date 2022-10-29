#include "Functions/SingleVariableMonomial.h"
#include "Utilities/MessageHandler.h"


#include <cmath>

SingleVariableMonomial::SingleVariableMonomial(int pow) : m_pow(pow)
{
	MessageHandler::Assert(m_pow >= 0, "Expect non-negative power in monomial");
}

double SingleVariableMonomial::Evaluate(double x)const
{
	return m_pow > 0 ? std::pow(x, m_pow) : 1.0;
}

double SingleVariableMonomial::Derivative(double x)const
{
	if (m_pow == 0)
	{
		return 0;
	}
	else if (m_pow == 1)
	{
		return 1;
	}
	else
	{
		return m_pow * std::pow(x, m_pow - 1);
	}
}

int SingleVariableMonomial::GetPower() const
{
	return m_pow;
}

bool SingleVariableMonomial::IsNonConstant() const
{
	return m_pow > 0;
}


bool SingleVariableMonomial::HasDerivative() const
{
	return true;
}
