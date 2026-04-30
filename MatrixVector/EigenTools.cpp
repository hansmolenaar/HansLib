#include "EigenTools.h"

#include "Defines.h"
#include "Functors.h"

namespace
{
using EigenColumnVector = Eigen::Matrix<double, Eigen::Dynamic, 1>;
using EigenMapVectorType = Eigen::Map<EigenColumnVector>;
using EigenMapVectorTypeConst = Eigen::Map<const EigenColumnVector>;
} // namespace

void EigenTools::TimesVector(const Eigen::MatrixXd &matrix, std::span<const double> vecin, std::span<double> result)
{
    EigenMapVectorType resultMapped(result.data(), result.size());
    EigenMapVectorTypeConst vecinMapped(vecin.data(), vecin.size());

    resultMapped = matrix * vecinMapped;
}

bool EigenTools::CheckConvergenceSolve(const Eigen::MatrixXd &matrix, std::span<const double> sol,
                                       std::span<const double> rhs)
{
    const double rhsNormSquared = Functors::SumOfSquares{}(rhs);
    std::vector<double> rsd(rhs.size());
    TimesVector(matrix, sol, rsd);
    constexpr double smallRelativeValue = 1.0e-10;
    std::transform(rsd.begin(), rsd.end(), rhs.begin(), rsd.begin(), std::minus<double>());
    const auto rsdNormSquared = Functors::SumOfSquares{}(rsd);
    return (rsdNormSquared / rhsNormSquared < smallRelativeValue);
}

bool EigenTools::CheckSolveRhsIsZero(const Eigen::MatrixXd &matrix, std::span<const double> rhs)
{
    const double rhsNormSquared = Functors::SumOfSquares{}(rhs);
    constexpr double reallySmallPositiveValue = 10 * std::numeric_limits<double>::min();
    return (rhsNormSquared <= reallySmallPositiveValue);
}
