#include "MatrixVector/RowColOfMatrix.h"
#include "HLUtils/MessageHandler.h"
#include "MatrixVector/IMatrix.h"

#include <algorithm>


RowColOfMatrix::RowColOfMatrix(IMatrix& matrix, int dim, bool isRow, int startRow, int startCol) :
	m_matrix(matrix), m_ofset({ startRow, startCol }), m_dimension(dim), m_isRow(isRow)
{
	MessageHandler::Assert(m_dimension >= 0);
	MessageHandler::Assert(*std::min_element(m_ofset.begin(), m_ofset.end()) >= 0);
	if (m_isRow)
	{
		MessageHandler::Assert(m_ofset[0] < matrix.GetRowDimension());
		MessageHandler::Assert(m_ofset[1] + m_dimension < matrix.GetColDimension());
	}
	else
	{
		MessageHandler::Assert(m_ofset[1] < matrix.GetColDimension());
		MessageHandler::Assert(m_ofset[0] + m_dimension < matrix.GetRowDimension());
	}
}



int RowColOfMatrix::GetDimension() const
{
	return m_dimension;
}


double RowColOfMatrix::operator() (int indx)  const
{
	MessageHandler::Assert(indx >= 0 && indx < m_dimension);
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
	MessageHandler::Assert(indx >= 0 && indx < m_dimension);
	if (m_isRow)
	{
		return m_matrix(m_ofset[0], m_ofset[1] + indx);
	}
	else
	{
		return m_matrix(m_ofset[0] + indx, m_ofset[1]);
	}
}

