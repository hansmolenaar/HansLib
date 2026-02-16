#include "EigenTools.h"

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
