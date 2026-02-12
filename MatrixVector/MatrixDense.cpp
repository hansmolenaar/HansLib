#include "MatrixDense.h"
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
    const auto rdim = GetRowDimension();
    const auto cdim = GetColDimension();
    Utilities::MyAssert(static_cast<int>(vecin.size()) == cdim);
    Utilities::MyAssert(static_cast<int>(result.size()) == rdim);
    str::fill(result, 0.0);

    for (auto r : Iota::GetRange(rdim))
    {
        for (auto c : Iota::GetRange(cdim))
        {
            result[r] += (*this)(r, c) * vecin[c];
        }
    }
}
