#include "Functions/IRealFunctionUtils.h"
#include "Functions/IRealFunction.h"
#include "MatrixVector/MatrixDense.h"
#include "Functions/SingleVariableRealValuedFunction.h"
#include "Functions/ISingleVariableRealValuedFunctionUtils.h"
#include "Utilities/Assert.h"
#include "Functions/MultiVariableRealValuedFunction.h"
#include "Functions/RealFunctionAdd.h"
#include "MatrixVector/IMatrixUtils.h"

#include <vector>

int GetDimension(const IRealFunction& fie)
{
	const int result = fie.GetDomainDimension();
	Utilities::Assert(result == fie.GetRangeDimension());
	return result;
}


double Evaluate(const IRealFunction& fie, std::span< const double> x)
{
	double result;
	fie.Evaluate(x, std::span(&result, 1));
	return result;
}



void  Derivative(const IRealFunction& fie, std::span< const double> x, std::span<double> dfdx)
{
	const int dim = fie.GetDomainDimension();
	MatrixDense jacob(1, dim);
	fie.Derivative(x, jacob);
	for (int n = 0; n < dim; ++n)
	{
		dfdx[n] = jacob(0, n);
	}
}

void CheckDerivatives(const IRealFunction& system, std::span< const double> x, std::span< const double> delx)
{
	const int numEqn = system.GetRangeDimension();
	const int numVar = system.GetDomainDimension();
	Utilities::Assert(x.size() == numVar);
	Utilities::Assert(delx.size() == numVar);
	std::vector<double> y(numEqn);
	MatrixDense jacobian(numEqn, numVar);
	for (int eqn = 0; eqn < numEqn; ++eqn)
	{
		for (int var = 0; var < numVar; ++var)
		{
			std::vector<double> vals(x.begin(), x.end());
			if (system.DerivativeAlwaysZero(eqn, var))
			{
				SetAll(jacobian, 1.0);
				system.Derivative(vals, jacobian);
				Utilities::Assert(jacobian(eqn, var) == 0.0);
			}
			else
			{
				std::function<double(double)> evaluate = [&](double xx) {vals[var] = xx;   system.Evaluate(vals, y); return y[eqn]; };
				std::function<double(double)> derivative = [&](double xx) {vals[var] = xx; system.Derivative(vals, jacobian); return jacobian(eqn, var); };
				SingleVariableRealValuedFunction fie(evaluate, derivative);
				ISingleVariableRealValuedFunctionUtils::CheckDerivative(fie, x[var], delx[var]);
			}

		}
	}
}


void CheckIndices(const IRealFunction& fie, int eqn, int var)
{
	Utilities::Assert(eqn >= 0 && eqn < fie.GetRangeDimension());
	Utilities::Assert(var >= 0 && var < fie.GetDomainDimension());
}


std::shared_ptr< IRealFunction> operator+(const IRealFunction& fie0, const IRealFunction& fie1)
{
	return std::make_unique<RealFunctionAdd>(fie0, fie1);
}
