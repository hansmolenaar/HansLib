#include "Functions/RealFunctionAdd.h"

#include "MyAssert.h"
#include "MatrixVector/MatrixDense.h"



RealFunctionAdd::RealFunctionAdd(const IRealFunction& fie1, const IRealFunction& fie2) : 
	m_fie1(fie1), m_fie2(fie2)
{
	Utilities::MyAssert(m_fie1.GetDomainDimension() == m_fie2.GetDomainDimension());
	Utilities::MyAssert(m_fie1.GetRangeDimension() == m_fie2.GetRangeDimension());
}


int RealFunctionAdd::GetRangeDimension() const
{
	return m_fie1.GetRangeDimension();
}


int RealFunctionAdd::GetDomainDimension() const
{
	return m_fie1.GetDomainDimension();
}


bool RealFunctionAdd::HasDerivative() const
{
	return true;
}

void RealFunctionAdd::Derivative(std::span<const double>x, IMatrix& dfdx)const
{
	m_fie1.Derivative(x, dfdx);
	MatrixDense dd(m_fie1.GetRangeDimension(), m_fie1.GetDomainDimension());
	m_fie2.Derivative(x, dd);
	for (int eqn = 0; eqn < m_fie2.GetRangeDimension(); ++eqn)
	{
		for (int var = 0; var < m_fie2.GetDomainDimension(); ++var)
		{
			if (!m_fie2.DerivativeAlwaysZero(eqn, var))
			{
				dfdx(eqn, var) += dd(eqn,var);
			}
		}
	}
}


void RealFunctionAdd::Evaluate(std::span<const double>x, std::span< double> y)const
{
	m_fie1.Evaluate(x, y);
	std::vector<double> yy(y.size());
	m_fie2.Evaluate(x, yy);
	for (auto n = 0; n < y.size(); ++n)
	{
		y[n] += yy[n];
	}
}

bool RealFunctionAdd::DerivativeAlwaysZero(int eqn, int var) const
{
	return m_fie1.DerivativeAlwaysZero(eqn, var) && m_fie2.DerivativeAlwaysZero(eqn, var);
}
