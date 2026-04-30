#include "MatrixNull.h"

#include <stdexcept>

int MatrixNull::GetRowDimension() const
{
    return 0;
}

int MatrixNull::GetColDimension() const
{
    return 0;
}

double MatrixNull::get(int row, int col) const
{
    throw std::runtime_error("Do not call me");
}

void MatrixNull::set(int, int, double)
{
    throw std::runtime_error("Do not call me");
}

void MatrixNull::timesVector(std::span<const double> vecin, std::span<double> result) const
{
    Utilities::MyAssert(vecin.empty());
    Utilities::MyAssert(result.empty());
}
