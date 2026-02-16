#pragma once

#include <Eigen/Dense>

#include <vector>

namespace EigenTools
{
void TimesVector(const Eigen::MatrixXd &, std::span<const double>, std::span<double>);
}
