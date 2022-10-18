#include "Functions/SingleVariableRealValuedFunction.h"
#include "HLUtils/MessageHandler.h"




SingleVariableRealValuedFunction::SingleVariableRealValuedFunction(
	std::function<double(double)> function,
	std::function<double(double)> derivative) :
	m_function(function), m_derivative(derivative)
{
}

SingleVariableRealValuedFunction::SingleVariableRealValuedFunction(std::function<double(double)> function) :
	m_function(function)
{
}

double SingleVariableRealValuedFunction::Evaluate(double x)const
{
	return m_function(x);
}

double SingleVariableRealValuedFunction::Derivative(double x)const
{
	return m_derivative(x);
}

bool SingleVariableRealValuedFunction::IsNonConstant() const
{
	return true;
}

bool SingleVariableRealValuedFunction::HasDerivative() const
{
	return m_derivative != nullptr;
}
