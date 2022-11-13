#include "MatrixVector/IMatrixSquareUtils.h"

#include "MatrixVector/IMatrixSquare.h"
#include "Utilities/Assert.h"

#include <algorithm>
#include <ranges>
#include <numeric>

namespace
{
	double GetDeterminant2x2(double a00, double a01, double a10, double a11)
	{
		return a00 * a11 - a10 * a01;
	}

	static int GetSubMatrixIndexLo3x3(int ind)
	{
		return ind == 0 ? 1 : 0;
	}

	static int GetSubMatrixIndexHi3x3(int ind)
	{
		return ind == 2 ? 1 : 2;
	}

	double GetSubDeterminant3x3(const IMatrixSquare& m, int row, int col)
	{
		const int r0 = GetSubMatrixIndexLo3x3(row);
		const int r1 = GetSubMatrixIndexHi3x3(row);
		const int c0 = GetSubMatrixIndexLo3x3(col);
		const int c1 = GetSubMatrixIndexHi3x3(col);
		return GetDeterminant2x2(m(r0, c0), m(r0, c1), m(r1, c0), m(r1, c1));
	}


}




double GetDeterminant(const IMatrixSquare& m)
{
	switch (m.GetDimension())
	{
	case 1:
		return m(0, 0);
	case 2:
		return m(0, 0)*m(1, 1) - m(0, 1)*m(1, 0);
	case 3:
		return
			m(0, 0)*GetSubDeterminant3x3(m, 0, 0) -
			m(1, 0)*GetSubDeterminant3x3(m, 1, 0) +
			m(2, 0)*GetSubDeterminant3x3(m, 2, 0);
	default:
		throw std::exception("Not implemented");
	}
}


double GetDeterminantDerivative(const IMatrixSquare& m, int row, int col)
{
	Utilities::Assert(std::min(row, col) >= 0 && std::max(row, col) < m.GetDimension());

	switch (m.GetDimension())
	{
	case 1:
		return 1.0;
	case 2:
	{
		const int r = (row + 1) % 2;
		const int c = (col + 1) % 2;
		const double factor = (row + col) % 2 == 0 ? 1.0 : -1.0;
		return factor * m(r, c);
	}
	case 3:
	{
		const double factor = (row + col) % 2 == 0 ? 1.0 : -1.0;
		return factor * GetSubDeterminant3x3(m, row, col);
	}
	default:
		throw std::exception("Not implemented");
	}
}

double GetTrace(const IMatrixSquare& matrix)
{
	double result = 0;
	for (int n = 0; n < matrix.GetDimension(); ++n)
	{
		result += matrix(n, n);
	}
	return result;
}


bool IsOrthogonal(const IMatrixSquare& matrix, double tol )
{
   const int dim = matrix.GetDimension();

   for (int row = 0; row < dim; ++row)
   {
      for (int col = 0; col < dim; ++col)
      {
         double val = 0;
         for (int k = 0; k < dim; ++k)
         {
            val += matrix(row, k)*matrix(col, k);
         }
         const double expect = row == col ? 1 : 0;
         if (std::abs(val - expect) > tol)
         {
            return false;
         }
      }
   }
   return true;
}

