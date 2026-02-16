#include "MatrixDense.h"
#include "EigenTools.h"
#include "IMatrixUtils.h"
#include "Iota.h"

MatrixDense::MatrixDense(int numRows, int numCols) : m_matrix(numRows, numCols)
{
    m_matrix.setZero();
}

int MatrixDense::GetRowDimension() const
{
    return m_matrix.rows();
}

int MatrixDense::GetColDimension() const
{
    return m_matrix.cols();
}

double MatrixDense::operator()(int row, int col) const
{
    return m_matrix(row, col);
}

double &MatrixDense::operator()(int row, int col)
{
    return m_matrix(row, col);
}

double MatrixDense::get(int row, int col) const
{
    return m_matrix(row, col);
}

void MatrixDense::set(int row, int col, double value)
{
    m_matrix(row, col) = value;
}

void MatrixDense::timesVector(std::span<const double> vecin, std::span<double> result) const
{
    EigenTools::TimesVector(m_matrix, vecin, result);
}
