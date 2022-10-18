#include "Functions/MultiVariableRealValuedFunction.h"
#include "HLUtils/MessageHandler.h"
#include "MatrixVector/IMatrix.h"
#include "MatrixVector/MatrixDense.h"
#include "MatrixVector/IMatrixUtils.h"
#include "Functions/IRealFunctionUtils.h"


MultiVariableRealValuedFunction::MultiVariableRealValuedFunction(
	std::vector<bool>  isactive,
	std::function<double(std::span<const double>)> function,
	std::function<void(std::span<const double>, std::span< double>)> derivative) :
	m_isActive(isactive), m_function(function), m_derivative(derivative)

{
	MessageHandler::Assert(m_isActive.size() > 0);
}

MultiVariableRealValuedFunction MultiVariableRealValuedFunction::CreateNull(int numVar)
{
	MessageHandler::Assert(numVar > 0);
	auto active = std::vector<bool>(numVar, false);
	return MultiVariableRealValuedFunction(
		active,
		[](std::span<const double>) {return 0.0; },
		[](std::span<const double>, std::span< double>jac) {std::fill(jac.begin(), jac.end(), 0.0); });
}

std::shared_ptr<IMultiVariableRealValuedFunction>  MultiVariableRealValuedFunction::CreateNullShared(int numVar)
{
	auto nul = new MultiVariableRealValuedFunction(MultiVariableRealValuedFunction::CreateNull(numVar));
	return std::shared_ptr<IMultiVariableRealValuedFunction>(nul);
}

int MultiVariableRealValuedFunction::GetDomainDimension() const
{
	return m_isActive.size();
}


bool MultiVariableRealValuedFunction::DerivativeAlwaysZero(int var) const
{
	return !m_isActive.at(var);
}


bool MultiVariableRealValuedFunction::HasDerivative() const
{
	return true;
}

double MultiVariableRealValuedFunction::Evaluate(std::span<const double>x) const
{
	MessageHandler::Assert(x.size() == GetDomainDimension());
	return m_function(x);
}

void MultiVariableRealValuedFunction::Derivative(std::span<const double> x, std::span< double> dfdx) const
{
	const int dim = GetDomainDimension();
	MessageHandler::Assert(x.size() == dim);
	MessageHandler::Assert(dfdx.size() == dim);
	std::vector<double> deriv(dim);
	m_derivative(x, deriv);
	for (int n = 0; n < dim; ++n)
	{
		dfdx[n] = deriv[n];
	}

}



MultiVariableRealValuedFunction operator-(const IMultiVariableRealValuedFunction& f, const IMultiVariableRealValuedFunction& g)
{
	MessageHandler::Assert(f.GetDomainDimension() == g.GetDomainDimension());
	const int dim = f.GetDomainDimension();

	std::vector<bool> active(dim);
	for (int n = 0; n < dim; ++n)
	{
		active[n] = !(f.DerivativeAlwaysZero(n) && g.DerivativeAlwaysZero(n));

	}
	std::function<double(std::span<const double>)> eval = [&](std::span<const double> x)
	{
		const double evalF = f.Evaluate(x);
		const double evalG = g.Evaluate(x);
		return evalF - evalG;
	};

	std::function<void(std::span<const double>, std::span< double>)> deriv = [&](std::span<const double> x, std::span< double> df)
	{
		MessageHandler::Assert(x.size() == df.size());
		const int dim = x.size();

		std::vector<double> dg(dim);
		f.Derivative(x, df);
		g.Derivative(x, dg);

		for (int n = 0; n < dim; ++n)
		{
			df[n] -= dg[n];
		}
	};

	return  MultiVariableRealValuedFunction(active, eval, deriv);
}

MultiVariableRealValuedFunction operator/(const IMultiVariableRealValuedFunction& numer, const IMultiVariableRealValuedFunction& denom)
{
	MessageHandler::Assert(numer.GetDomainDimension() == denom.GetDomainDimension());
	const int dim = numer.GetDomainDimension();

	std::vector<bool> active(dim);
	for (int n = 0; n < dim; ++n)
	{
		active[n] = !(numer.DerivativeAlwaysZero(n) && denom.DerivativeAlwaysZero(n));

	}
	std::function<double(std::span<const double>)> eval = [&](std::span<const double> x)
	{
		double n = Evaluate(numer, x);
		double d = Evaluate(denom, x);
		return n / d;
	};

	std::function<void(std::span<const double>, std::span< double>)> deriv = [&](std::span<const double> x, std::span< double> df)
	{
		MessageHandler::Assert(x.size() == df.size());
		const int dim = x.size();

		const double evalNumer = Evaluate(numer, x);
		const double evalDenom = Evaluate(denom, x);

		std::vector<double> dnumer(dim);
		std::vector<double> ddenom(dim);
		Derivative(numer, x, dnumer);
		Derivative(denom, x, ddenom);

		const double d = 1 / (evalDenom*evalDenom);
		for (int n = 0; n < dim; ++n)
		{
			df[n] = (dnumer[n] * evalDenom - evalNumer * ddenom[n])*d;
		}
	};

	return  MultiVariableRealValuedFunction(active, eval, deriv);
}
