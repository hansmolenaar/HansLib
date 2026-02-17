#include "MatrixDenseSymmetric.h"

#include "EigenTools.h"
#include "IMatrixUtils.h"
#include "Iota.h"
#include "MyAssert.h"

using namespace Utilities;

namespace
{
using EigenColumnVector = Eigen::Matrix<double, Eigen::Dynamic, 1>;
using EigenMapVectorType = Eigen::Map<EigenColumnVector>;
using EigenMapVectorTypeConst = Eigen::Map<const EigenColumnVector>;
} // namespace

MatrixDenseSymmetric::MatrixDenseSymmetric(int dim) : m_matrix(dim, dim)
{
    CheckDimensions(*this);
    m_matrix.setZero();
}

int MatrixDenseSymmetric::GetDimension() const
{
    return m_matrix.rows();
}

double MatrixDenseSymmetric::operator()(int row, int col) const
{
    CheckRowCol(*this, row, col);
    return m_matrix(row, col);
}

double MatrixDenseSymmetric::get(int row, int col) const
{
    return (*this)(row, col);
}

void MatrixDenseSymmetric::set(int row, int col, double value)
{
    CheckRowCol(*this, row, col);
    m_matrix(row, col) = value;
    m_matrix(col, row) = value;
}

void MatrixDenseSymmetric::timesVector(std::span<const double> vecin, std::span<double> result) const
{
    EigenTools::TimesVector(m_matrix, vecin, result);
}

bool MatrixDenseSymmetric::Solve(std::span<const double> rhs, std::span<double> sol)
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

EigenSolution MatrixDenseSymmetric::getEigenSolution()
{
    const auto dim = GetDimension();
    Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd> eigensolver(m_matrix);
    MyAssert(eigensolver.info() == Eigen::Success);
    std::vector<double> eigenvalues;
    for (auto n : Iota::GetRange(dim))
    {
        eigenvalues.push_back(eigensolver.eigenvalues()(n));
    }
    return EigenSolution{eigenvalues};
}
