#include "Functions/RealFunctionNull.h"

#include "Utilities/MyAssert.h"
#include "Functions/IRealFunctionUtils.h"
#include "MatrixVector/IMatrix.h"
#include "MatrixVector/IMatrixUtils.h"

#include <algorithm>

RealFunctionNull::RealFunctionNull(int numVar, int numEqn) :
	m_numEqn(numEqn), m_numVar(numVar)
{
	Utilities::MyAssert(m_numVar > 0);
	Utilities::MyAssert(m_numEqn > 0);
}


int RealFunctionNull::GetRangeDimension() const
{
	return m_numEqn;
}


int RealFunctionNull::GetDomainDimension() const
{
	return m_numVar;
}


bool RealFunctionNull::HasDerivative() const
{
	return true;
}

void RealFunctionNull::Derivative(std::span<const double>x, IMatrix& dfdx)const
{
	Utilities::MyAssert(m_numVar == dfdx.GetColDimension());
	Utilities::MyAssert(m_numEqn == dfdx.GetRowDimension());
	Clear(dfdx);
}


void RealFunctionNull::Evaluate(std::span<const double>x, std::span< double> y)const
{
	Utilities::MyAssert(x.size() == GetDomainDimension());
	Utilities::MyAssert(y.size() == GetRangeDimension());
	std::fill_n(y.begin(), GetRangeDimension(), 0.0);
}

bool RealFunctionNull::DerivativeAlwaysZero(int eqn, int var) const
{
	CheckIndices(*this, eqn, var);
	return true;
}
