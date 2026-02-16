#include "MatrixDenseSymmetric.h"

#include "EigenTools.h"
#include "IMatrixUtils.h"
#include "Iota.h"

MatrixDenseSymmetric::MatrixDenseSymmetric(int dim) : m_matrix(dim, dim)
{
}

int MatrixDenseSymmetric::GetDimension() const
{
    return m_matrix.rows();
}

double MatrixDenseSymmetric::operator()(int row, int col) const
{
    CheckRowCol(*this, row, col);
    return m_matrix(row, col);
}

double MatrixDenseSymmetric::get(int row, int col) const
{
    return (*this)(row, col);
}

void MatrixDenseSymmetric::set(int row, int col, double value)
{
    CheckRowCol(*this, row, col);
    m_matrix(row, col) = value;
    m_matrix(col, row) = value;
}

void MatrixDenseSymmetric::timesVector(std::span<const double> vecin, std::span<double> result) const
{
    EigenTools::TimesVector(m_matrix, vecin, result);
}
