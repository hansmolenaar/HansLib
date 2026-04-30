#include "MatrixSquare.h"
#include "Defines.h"
#include "EigenTools.h"
#include "Functors.h"
#include "IMatrixUtils.h"
#include "Iota.h"
#include "MyAssert.h"

#include <iostream>
#include <limits>

namespace
{
using EigenColumnVector = Eigen::Matrix<double, Eigen::Dynamic, 1>;
using EigenMapVectorType = Eigen::Map<EigenColumnVector>;
using EigenMapVectorTypeConst = Eigen::Map<const EigenColumnVector>;
} // namespace

MatrixSquare::MatrixSquare(int dim) : m_matrix(dim, dim)
{
    CheckDimensions(*this);
    m_matrix.setZero();
}

double MatrixSquare::operator()(int row, int col) const
{
    CheckRowCol(*this, row, col);
    return m_matrix(row, col);
}

double MatrixSquare::get(int row, int col) const
{
    return (*this)(row, col);
}

double &MatrixSquare::operator()(int row, int col)
{
    CheckRowCol(*this, row, col);
    return m_matrix(row, col);
}

void MatrixSquare::set(int row, int col, double value)
{
    (*this)(row, col) = value;
}

int MatrixSquare::GetDimension() const
{
    return m_matrix.rows();
}

bool MatrixSquare::Solve(std::span<const double> rhs, std::span<double> sol)
{
    Utilities::MyAssert(static_cast<int>(rhs.size()) == GetDimension());
    Utilities::MyAssert(static_cast<int>(sol.size()) == GetDimension());
    if (EigenTools::CheckSolveRhsIsZero(m_matrix, rhs))
    {
        str::fill(sol, 0.0);
        return true;
    }

    const auto factorization = m_matrix.householderQr();
    if (factorization.info() != Eigen::ComputationInfo::Success)
    {
        return false;
    }

    EigenMapVectorType solMapped(sol.data(), sol.size());
    EigenMapVectorTypeConst rhsMapped(rhs.data(), rhs.size());
    solMapped = factorization.solve(rhsMapped);

    // Check solution
    return EigenTools::CheckConvergenceSolve(m_matrix, sol, rhs);
}

void MatrixSquare::timesVector(std::span<const double> vecin, std::span<double> result) const
{
    EigenTools::TimesVector(m_matrix, vecin, result);
}
