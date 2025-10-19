#include "IRealFunctionUtils.h"
#include "IRealFunction.h"
#include "MatrixDense.h"
#include "SingleVariableRealValuedFunction.h"
#include "ISingleVariableRealValuedFunctionUtils.h"
#include "MyAssert.h"
#include "MultiVariableRealValuedFunction.h"
#include "RealFunctionAdd.h"
#include "IMatrixUtils.h"

#include <vector>

int GetDimension(const IRealFunction& fie)
{
	const int result = fie.GetDomainDimension();
	Utilities::MyAssert(result == fie.GetRangeDimension());
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

void CheckIndices(const IRealFunction& fie, int eqn, int var)
{
	Utilities::MyAssert(eqn >= 0 && eqn < fie.GetRangeDimension());
	Utilities::MyAssert(var >= 0 && var < fie.GetDomainDimension());
}


std::shared_ptr< IRealFunction> operator+(const IRealFunction& fie0, const IRealFunction& fie1)
{
	return std::make_unique<RealFunctionAdd>(fie0, fie1);
}
