#include "IMatrixKelvinReprUtils.h"

#include "IMatrixKelvinRepr.h"
#include "MyAssert.h"

void SetAll(IMatrixKelvinRepr &matrix, double value)
{
    const int dim = matrix.GetDimension();
    for (int row = 0; row < dim; ++row)
    {
        for (int col = row; col < dim; ++col)
        {
            matrix.Set(row, col, value);
        }
    }
}

void Clear(IMatrixKelvinRepr &matrix)
{
    SetAll(matrix, 0.0);
}

IMatrixKelvinRepr &operator*=(IMatrixKelvinRepr &matrix, double factor)
{
    const int dim = matrix.GetDimension();
    for (int row = 0; row < dim; ++row)
    {
        for (int col = row; col < dim; ++col)
        {
            const double val = matrix.Get(row, col);
            matrix.Set(row, col, factor * val);
        }
    }
    return matrix;
}

IMatrixKelvinRepr &operator+=(IMatrixKelvinRepr &matrix, const IMatrixKelvinRepr &matrixAdd)
{
    const int dim = matrix.GetDimension();
    for (int row = 0; row < dim; ++row)
    {
        for (int col = row; col < dim; ++col)
        {
            const double val = matrix.Get(row, col) + matrixAdd.Get(row, col);
            matrix.Set(row, col, val);
        }
    }
    return matrix;
}

#if false
void IMatrixRotation::RotateTensor(const IMatrixKelvinRepr& tin, IMatrixKelvinRepr& tout) const
{
   const int dim = GetDimension();
   Utilities::MyAssert(tin.GetDimension() == dim);
   Utilities::MyAssert(tout.GetDimension() == dim);
   for (int row = 0; row < dim; ++row)
   {
      for (int col = row; col < dim; ++col)
      {
         double sum = 0;
         for (int m = 0; m < dim; ++m)
         {
            for (int n = 0; n < dim; ++n)
            {
               sum += (*this)(m, row)*tin(m, n)*(*this)(n, col);
            }
         }
         tout.Set(row, col, sum);
      }
   }
}
#endif