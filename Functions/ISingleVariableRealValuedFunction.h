#pragma once

#include <span>

#include "IMultiVariableRealValuedFunction.h"
#include "MatrixVector/MatrixSquare.h"
#include "MyAssert.h"

class ISingleVariableRealValuedFunction :public IMultiVariableRealValuedFunction
{
public:
	virtual ~ISingleVariableRealValuedFunction() = default;

	virtual double Evaluate(double x)const = 0;
	virtual double Derivative(double x)const = 0;
	virtual bool IsNonConstant() const = 0;

	using IMultiVariableRealValuedFunction::Evaluate;
	using IMultiVariableRealValuedFunction::Derivative;

	inline int GetDomainDimension() const override;
	inline double Evaluate(std::span<const double>x)const override;
	inline void Derivative(std::span<const double>x, std::span< double> dfdx)const override;
	inline bool DerivativeAlwaysZero(int var) const override;
};


int ISingleVariableRealValuedFunction::GetDomainDimension() const
{
	return 1;
}



double ISingleVariableRealValuedFunction::Evaluate(std::span<const double> x)const
{
	Utilities::MyAssert(x.size() == 1);
	return Evaluate(x[0]);
}


void ISingleVariableRealValuedFunction::Derivative(std::span<const double>x, std::span< double> dfdx) const
{
	Utilities::MyAssert(x.size() == 1);
	Utilities::MyAssert(dfdx.size() == 1);
	dfdx[0] = Derivative(x[0]);
}

bool ISingleVariableRealValuedFunction::DerivativeAlwaysZero(int var) const
{
	Utilities::MyAssert(var == 0);
	return false;
}
