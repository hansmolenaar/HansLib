#include "MatrixDenseSymmetric.h"

#include "IMatrixUtils.h"

MatrixDenseSymmetric::MatrixDenseSymmetric(int dim) : m_dim(dim), m_indexer(dim), m_matrix(m_indexer.getFlatSize(), 0.0)
{
}

int MatrixDenseSymmetric::GetDimension() const
{
    return m_dim;
}

double MatrixDenseSymmetric::operator()(int row, int col) const
{
    return m_matrix.at(m_indexer.ToFlat(row, col));
}

double &MatrixDenseSymmetric::operator()(int row, int col)
{
    return m_matrix.at(m_indexer.ToFlat(row, col));
}

void MatrixDenseSymmetric::timesVector(std::span<const double> vecin, std::span<double> result) const
{
    MatrixTimesVector(*this, vecin, result);
}
