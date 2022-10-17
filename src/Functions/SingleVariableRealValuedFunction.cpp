#include "Functions/SingleVariableRealValuedFunction.h"
#include "Utils/MessageHandler.h"




SingleVariableRealValuedFunction::SingleVariableRealValuedFunction(
	std::function<double(double)> function,
	std::function<double(double)> derivative) :
	m_function(function), m_derivative(derivative)
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