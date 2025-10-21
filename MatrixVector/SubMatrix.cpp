#include "SubMatrix.h"
#include "MyAssert.h"

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
