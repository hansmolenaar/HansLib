#include "IMatrixRotation.h"
#include "IMatrixSquareUtils.h"
#include "MyException.h"

void IMatrixRotation::set(int, int, double)
{
   throw MyException("Don't call me, use const version instead");
}

bool IMatrixRotation::IsRotation(const IMatrixSquare &matrix, double tol)
{
   if (!IsOrthogonal(matrix, tol))
   {
      return false;
   }
   const double det = GetDeterminant(matrix);
   return (std::abs(det - 1.0) < tol);
}
