#include "MatrixVector/IMatrixUtils.h"

#include "MatrixVector/IMatrix.h"
#include "Utilities/Assert.h"

#include <span>

void CheckDimensions(const IMatrix& matrix)
{
	Utilities::Assert(matrix.GetRowDimension() >= 0);
	Utilities::Assert(matrix.GetColDimension() >= 0);
}


void CheckRowCol(const IMatrix& matrix, int row, int col)
{
	Utilities::Assert(row >= 0 && row < matrix.GetRowDimension()); 
	Utilities::Assert(col >= 0 && col < matrix.GetColDimension());
}

IMatrix& operator*=(IMatrix& matrix, double factor)
{
	for (int row = 0; row < matrix.GetRowDimension(); ++row)
	{
		for (int col = 0; col < matrix.GetColDimension(); ++col)
		{
			matrix(row, col) = factor * matrix(row, col);
		}
	}
	return matrix;
}

IMatrix& operator+=(IMatrix& matrix, const IMatrix& matrixToAdd)
{
	Utilities::Assert(matrix.GetRowDimension() == matrixToAdd.GetRowDimension());
	Utilities::Assert(matrix.GetColDimension() == matrixToAdd.GetColDimension());

	for (int row = 0; row < matrix.GetRowDimension(); ++row)
	{
		for (int col = 0; col < matrix.GetColDimension(); ++col)
		{
			matrix(row, col) += matrixToAdd(row, col);
		}
	}
	return matrix;
}

void SetAll(IMatrix& matrix, double value)
{
	for (int row = 0; row < matrix.GetRowDimension(); ++row)
	{
		for (int col = 0; col < matrix.GetColDimension(); ++col)
		{
			matrix(row, col) = value;
		}
	}
}



void Clear(IMatrix& matrix)
{
	SetAll(matrix, 0.0);
}


void MatrixTimesVector(const IMatrix& matrix, std::span<const double> vecin, std::span<double> vecout)
{
   Utilities::Assert(matrix.GetRowDimension() == vecout.size());
   Utilities::Assert(matrix.GetColDimension() == vecin.size());
   for (int row = 0; row < matrix.GetRowDimension(); ++row)
   {
      double sum = 0;
      for (int col = 0; col < matrix.GetColDimension(); ++col)
      {
         sum += matrix(row, col)*vecin[col];
      }
      vecout[row] = sum;
   }
}


// mat3 = mat1*mat2
void MatrixTimesMatrix(const IMatrix& mat1, const IMatrix& mat2, IMatrix& mat3)
{
   Utilities::Assert(mat1.GetRowDimension() == mat3.GetRowDimension());
   Utilities::Assert(mat2.GetColDimension() == mat3.GetColDimension());
   Utilities::Assert(mat1.GetColDimension() == mat2.GetRowDimension());
   for (int row = 0; row < mat1.GetRowDimension(); ++row)
   {
      for (int col = 0; col < mat2.GetColDimension(); ++col)
      {
         double sum = 0;
         for (int k = 0; k < mat1.GetColDimension(); ++k)
         {
            sum += mat1(row, k)*mat2(k, col);
         }
         mat3(row, col) = sum;
      }
   }
}
