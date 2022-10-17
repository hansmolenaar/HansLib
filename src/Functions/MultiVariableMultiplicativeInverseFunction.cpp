#include "Functions/MultiVariableMultiplicativeInverseFunction.h"
#include "Utils/MessageHandler.h"

#include <algorithm>

MultiVariableMultiplicativeInverseFunction::MultiVariableMultiplicativeInverseFunction(const IMultiVariableRealValuedFunction& fie) :
	m_function(fie)
{
}

int MultiVariableMultiplicativeInverseFunction::GetDomainDimension() const
{
	return m_function.GetDomainDimension();
}




bool MultiVariableMultiplicativeInverseFunction::DerivativeAlwaysZero(int var) const
{
	return m_function.DerivativeAlwaysZero(var);
}

double MultiVariableMultiplicativeInverseFunction::Evaluate(std::span<const double>x) const
{
	double fie = m_function.Evaluate(x);
	MessageHandler::Assert(fie != 0.0);
	return 1 / fie;
}

void MultiVariableMultiplicativeInverseFunction::Derivative(std::span<const double>x, std::span< double> dfdx)const
{
	const int dim = GetDomainDimension();
	const double eval = m_function.Evaluate(x);
	const double eval2 = eval * eval;
	m_function.Derivative(x, dfdx);
	for (int var = 0; var < dim; ++var)
	{
		if (DerivativeAlwaysZero(var))
		{
			dfdx[var] = 0;
		}
		else
		{
			MessageHandler::Assert(eval2 != 0.0);
			dfdx[var] = -dfdx[var] / eval2;
		}
	}
}