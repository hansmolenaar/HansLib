#include "IMatrixUtils.h"

#include "IMatrix.h"
#include "MyAssert.h"

#include <span>

void CheckDimensions(const IMatrix &matrix)
{
    Utilities::MyAssert(matrix.GetRowDimension() >= 0);
    Utilities::MyAssert(matrix.GetColDimension() >= 0);
}

void CheckRowCol(const IMatrix &matrix, int row, int col)
{
    Utilities::MyAssert(row >= 0 && row < matrix.GetRowDimension());
    Utilities::MyAssert(col >= 0 && col < matrix.GetColDimension());
}

IMatrix &operator*=(IMatrix &matrix, double factor)
{
    for (int row = 0; row < matrix.GetRowDimension(); ++row)
    {
        for (int col = 0; col < matrix.GetColDimension(); ++col)
        {
            matrix.set(row, col, factor * matrix.get(row, col));
        }
    }
    return matrix;
}

IMatrix &operator+=(IMatrix &matrix, const IMatrix &matrixToAdd)
{
    Utilities::MyAssert(matrix.GetRowDimension() == matrixToAdd.GetRowDimension());
    Utilities::MyAssert(matrix.GetColDimension() == matrixToAdd.GetColDimension());

    for (int row = 0; row < matrix.GetRowDimension(); ++row)
    {
        for (int col = 0; col < matrix.GetColDimension(); ++col)
        {
            matrix.add(row, col, matrixToAdd.get(row, col));
        }
    }
    return matrix;
}

void SetAll(IMatrix &matrix, double value)
{
    for (int row = 0; row < matrix.GetRowDimension(); ++row)
    {
        for (int col = 0; col < matrix.GetColDimension(); ++col)
        {
            matrix.set(row, col, value);
        }
    }
}

void Clear(IMatrix &matrix)
{
    SetAll(matrix, 0.0);
}

void MatrixTimesVector(const IMatrix &matrix, std::span<const double> vecin, std::span<double> vecout)
{
    const auto rdim = matrix.GetRowDimension();
    const auto cdim = matrix.GetColDimension();
    Utilities::MyAssert(static_cast<int>(vecin.size()) == cdim);
    Utilities::MyAssert(static_cast<int>(vecout.size()) == rdim);

    for (int row = 0; row < matrix.GetRowDimension(); ++row)
    {
        double sum = 0;
        for (int col = 0; col < matrix.GetColDimension(); ++col)
        {
            sum += matrix.get(row, col) * vecin[col];
        }
        vecout[row] = sum;
    }
}

// mat3 = mat1*mat2
void MatrixTimesMatrix(const IMatrix &mat1, const IMatrix &mat2, IMatrix &mat3)
{
    Utilities::MyAssert(mat1.GetRowDimension() == mat3.GetRowDimension());
    Utilities::MyAssert(mat2.GetColDimension() == mat3.GetColDimension());
    Utilities::MyAssert(mat1.GetColDimension() == mat2.GetRowDimension());
    for (int row = 0; row < mat1.GetRowDimension(); ++row)
    {
        for (int col = 0; col < mat2.GetColDimension(); ++col)
        {
            double sum = 0;
            for (int k = 0; k < mat1.GetColDimension(); ++k)
            {
                sum += mat1.get(row, k) * mat2.get(k, col);
            }
            mat3.set(row, col, sum);
        }
    }
}
