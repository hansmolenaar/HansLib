#include "SubMatrix.h"
#include "MyAssert.h"
#include "Defines.h"
#include "Iota.h"

#include "IMatrixUtils.h"

SubMatrix::SubMatrix(IMatrix &matrix, int dimRow, int dimCol, int ofsetRow, int ofsetCol)
    : m_matrix(matrix), m_ofset({ofsetRow, ofsetCol}), m_dimension({dimRow, dimCol})
{
    CheckDimensions(*this);
    Utilities::MyAssert(ofsetRow >= 0);
    Utilities::MyAssert(ofsetCol >= 0);
    Utilities::MyAssert(dimRow + ofsetRow <= matrix.GetRowDimension());
    Utilities::MyAssert(dimCol + ofsetCol <= matrix.GetColDimension());
}

int SubMatrix::GetRowDimension() const
{
    return m_dimension[0];
}

int SubMatrix::GetColDimension() const
{
    return m_dimension[1];
}

double SubMatrix::operator()(int row, int col) const
{
    CheckRowCol(*this, row, col);
    return m_matrix(row + m_ofset[0], col + m_ofset[1]);
}

double &SubMatrix::operator()(int row, int col)
{
    CheckRowCol(*this, row, col);
    return m_matrix(row + m_ofset[0], col + m_ofset[1]);
}

void SubMatrix::timesVector(std::span<const double> vecin, std::span<double> result) const
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
