#include "MatrixDenseSymmetric.h"

#include "IMatrixUtils.h"
#include "Iota.h"

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

double MatrixDenseSymmetric::get(int row, int col) const
{
    return (*this)(row, col);
}

double &MatrixDenseSymmetric::operator()(int row, int col)
{
    return m_matrix.at(m_indexer.ToFlat(row, col));
}

void MatrixDenseSymmetric::set(int row, int col, double value)
{
    (*this)(row, col) = value;
}

void MatrixDenseSymmetric::timesVector(std::span<const double> vecin, std::span<double> result) const
{
    const auto dim = GetDimension();
    Utilities::MyAssert(static_cast<int>(vecin.size()) == dim);
    Utilities::MyAssert(static_cast<int>(result.size()) == dim);

    // Diag
    for (auto ind : Iota::GetRange(dim))
    {
        result[ind] = (*this)(ind, ind) * vecin[ind];
    }

    for (auto row : Iota::GetRange(dim))
    {
        for (auto col : Iota::GetRangeFromTo(row + 1, dim))
        {
            const double entry = (*this)(row, col);
            result[row] += entry * vecin[col];
            result[col] += entry * vecin[row];
        }
    }
}
