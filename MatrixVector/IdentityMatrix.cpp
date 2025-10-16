#include "IdentityMatrix.h"

#include "MyAssert.h"
#include "IMatrixUtils.h"

#include <map>
#include <memory>

IdentityMatrix::IdentityMatrix(int dim) :
	m_dimension(dim)
{
	Utilities::MyAssert(m_dimension > 0);
}

const IdentityMatrix& IdentityMatrix::GetInstance(int dim)
{
	static std::map<int, std::unique_ptr< IdentityMatrix>> matrices;
	if (matrices.find(dim) == matrices.end())
	{
		matrices[dim] = std::make_unique<IdentityMatrix>(dim);
	}
	return *(matrices.find(dim)->second);
}


int IdentityMatrix::GetRowDimension() const
{
	return m_dimension;
}

int IdentityMatrix::GetColDimension() const
{
	return m_dimension;
}


int IdentityMatrix::GetDimension() const
{
	return m_dimension;
}


double IdentityMatrix::operator() (int row, int col)  const
{
	CheckRowCol(*this, row, col);
	return (row == col ? 1 : 0);
}


double& IdentityMatrix::operator() (int row, int col)
{
	throw std::runtime_error("Don't call me");
}

