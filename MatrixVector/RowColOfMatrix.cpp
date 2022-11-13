#include "MatrixVector/RowColOfMatrix.h"
#include "Utilities/Assert.h"
#include "MatrixVector/IMatrix.h"

#include <algorithm>


RowColOfMatrix::RowColOfMatrix(IMatrix& matrix, int dim, bool isRow, int startRow, int startCol) :
	m_matrix(matrix), m_ofset({ startRow, startCol }), m_dimension(dim), m_isRow(isRow)
{
	Utilities::Assert(m_dimension >= 0);
	Utilities::Assert(*std::min_element(m_ofset.begin(), m_ofset.end()) >= 0);
	if (m_isRow)
	{
		Utilities::Assert(m_ofset[0] < matrix.GetRowDimension());
		Utilities::Assert(m_ofset[1] + m_dimension < matrix.GetColDimension());
	}
	else
	{
		Utilities::Assert(m_ofset[1] < matrix.GetColDimension());
		Utilities::Assert(m_ofset[0] + m_dimension < matrix.GetRowDimension());
	}
}



int RowColOfMatrix::GetDimension() const
{
	return m_dimension;
}


double RowColOfMatrix::operator() (int indx)  const
{
	Utilities::Assert(indx >= 0 && indx < m_dimension);
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
	Utilities::Assert(indx >= 0 && indx < m_dimension);
	if (m_isRow)
	{
		return m_matrix(m_ofset[0], m_ofset[1] + indx);
	}
	else
	{
		return m_matrix(m_ofset[0] + indx, m_ofset[1]);
	}
}

