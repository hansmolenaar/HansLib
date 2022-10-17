#include "Functions/RealFunctionNull.h"

#include "Utils/MessageHandler.h"
#include "Functions/IRealFunctionUtils.h"
#include "MatrixVector/IMatrix.h"
#include "MatrixVector/IMatrixUtils.h"

#include <algorithm>

RealFunctionNull::RealFunctionNull(int numVar, int numEqn) :
	m_numEqn(numEqn), m_numVar(numVar)
{
	MessageHandler::Assert(m_numVar > 0);
	MessageHandler::Assert(m_numEqn > 0);
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
	MessageHandler::Assert(m_numVar == dfdx.GetColDimension());
	MessageHandler::Assert(m_numEqn == dfdx.GetRowDimension());
	Clear(dfdx);
}


void RealFunctionNull::Evaluate(std::span<const double>x, std::span< double> y)const
{
	MessageHandler::Assert(x.size() == GetDomainDimension());
	MessageHandler::Assert(y.size() == GetRangeDimension());
	std::fill_n(y.begin(), GetRangeDimension(), 0.0);
}

bool RealFunctionNull::DerivativeAlwaysZero(int eqn, int var) const
{
	CheckIndices(*this, eqn, var);
	return true;
}
