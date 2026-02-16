#pragma once

#include <Eigen/Dense>

#include <vector>

namespace EigenTools
{
void TimesVector(const Eigen::MatrixXd &, std::span<const double>, std::span<double>);
bool CheckConvergenceSolve(const Eigen::MatrixXd &, std::span<const double>, std::span<const double>);
bool CheckSolveRhsIsZero(const Eigen::MatrixXd &, std::span<const double>);
} // namespace EigenTools
