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

double MatrixNull::operator()(int row, int col) const
{
    throw std::runtime_error("Do not call me");
}

double &MatrixNull::operator()(int row, int col)
{
    throw std::runtime_error("Do not call me");
}
