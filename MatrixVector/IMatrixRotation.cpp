#include "IMatrixRotation.h"
#include "IMatrixSquareUtils.h"
#include "MyAssert.h"

double &IMatrixRotation::operator()(int, int)
{
    throw std::runtime_error("Don't call me, use const version instead");
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
