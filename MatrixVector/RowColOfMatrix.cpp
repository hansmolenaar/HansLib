#include "MatrixVector/RowColOfMatrix.h"
#include "MyAssert.h"
#include "MatrixVector/IMatrix.h"

#include <algorithm>


RowColOfMatrix::RowColOfMatrix(IMatrix& matrix, int dim, bool isRow, int startRow, int startCol) :
	m_matrix(matrix), m_ofset({ startRow, startCol }), m_dimension(dim), m_isRow(isRow)
{
	Utilities::MyAssert(m_dimension >= 0);
	Utilities::MyAssert(*std::min_element(m_ofset.begin(), m_ofset.end()) >= 0);
	if (m_isRow)
	{
		Utilities::MyAssert(m_ofset[0] < matrix.GetRowDimension());
		Utilities::MyAssert(m_ofset[1] + m_dimension < matrix.GetColDimension());
	}
	else
	{
		Utilities::MyAssert(m_ofset[1] < matrix.GetColDimension());
		Utilities::MyAssert(m_ofset[0] + m_dimension < matrix.GetRowDimension());
	}
}



int RowColOfMatrix::GetDimension() const
{
	return m_dimension;
}


double RowColOfMatrix::operator() (int indx)  const
{
	Utilities::MyAssert(indx >= 0 && indx < m_dimension);
	if (m_isRow)
	{
		return m_matrix(m_ofset[0], m_ofset[1] + indx);
	}
	else
	{
		return m_matrix(m_ofset[0] + indx, m_ofset[1]);
	}
}


double& RowColOfMatrix::operator() (int indx)
{
	Utilities::MyAssert(indx >= 0 && indx < m_dimension);
	if (m_isRow)
	{
		return m_matrix(m_ofset[0], m_ofset[1] + indx);
	}
	else
	{
		return m_matrix(m_ofset[0] + indx, m_ofset[1]);
	}
}

