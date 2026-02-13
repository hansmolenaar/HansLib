#include "IdentityMatrix.h"

#include "Defines.h"
#include "IMatrixUtils.h"
#include "MyAssert.h"

#include <map>
#include <memory>

IdentityMatrix::IdentityMatrix(int dim) : m_dimension(dim)
{
    Utilities::MyAssert(m_dimension > 0);
}

const IdentityMatrix &IdentityMatrix::GetInstance(int dim)
{
    static std::map<int, std::unique_ptr<IdentityMatrix>> matrices;
    if (matrices.find(dim) == matrices.end())
    {
        matrices[dim] = std::make_unique<IdentityMatrix>(dim);
    }
    return *(matrices.find(dim)->second);
}

int IdentityMatrix::GetDimension() const
{
    return m_dimension;
}

double IdentityMatrix::operator()(int row, int col) const
{
    CheckRowCol(*this, row, col);
    return (row == col ? 1 : 0);
}

void IdentityMatrix::set(int, int, double)
{
    throw std::runtime_error("Don't call me");
}

void IdentityMatrix::timesVector(std::span<const double> vecin, std::span<double> vecout) const
{
    Utilities::MyAssert(static_cast<int>(vecin.size()) == GetRowDimension());
    Utilities::MyAssert(static_cast<int>(vecout.size()) == GetRowDimension());
    str::copy(vecin, vecout.begin());
}
