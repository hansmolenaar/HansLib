#include "Functions/IMultiVariableRealValuedFunction.h"
#include "Utilities/Assert.h"

#include <vector>

int IMultiVariableRealValuedFunction::GetRangeDimension() const
{
	return 1;
}

void IMultiVariableRealValuedFunction::Evaluate(std::span<const double>x, std::span< double> y) const
{
	Utilities::Assert(y.size() == 1);
	y[0] = Evaluate(x);
}

bool IMultiVariableRealValuedFunction::DerivativeAlwaysZero(int eqn, int var) const
{
	Utilities::Assert(eqn == 0);
	return DerivativeAlwaysZero(var);
}


void IMultiVariableRealValuedFunction::Derivative(std::span<const double>x, IMatrix& dfdx) const
{
	Utilities::Assert(dfdx.GetRowDimension() == 1);
	const int dim = GetDomainDimension();
	Utilities::Assert(dfdx.GetColDimension() == dim);
	std::vector < double > deriv(dim);
	Derivative(x, deriv);
	for (int n = 0; n < dim; ++n)
	{
		dfdx(0, n) = deriv[n];
	}
}