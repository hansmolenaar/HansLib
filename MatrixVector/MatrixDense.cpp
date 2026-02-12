#include "MatrixDense.h"
#include "IMatrixUtils.h"
#include "Iota.h"

MatrixDense::MatrixDense(int numRows, int numCols) : m_indexer(numRows, numCols), m_entries(numRows * numCols)
{
}

int MatrixDense::GetRowDimension() const
{
    return m_indexer.GetRowDimension();
}

int MatrixDense::GetColDimension() const
{
    return m_indexer.GetColDimension();
}

double MatrixDense::operator()(int row, int col) const
{
    return m_entries[m_indexer.toFlat({row, col})];
}

double &MatrixDense::operator()(int row, int col)
{
    return m_entries[m_indexer.toFlat({row, col})];
}

void MatrixDense::timesVector(std::span<const double> vecin, std::span<double> result) const
{
    MatrixTimesVector(*this, vecin, result);
}
