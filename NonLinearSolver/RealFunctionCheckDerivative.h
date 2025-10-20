#include "ISingleVariableRealValuedFunction.h"

namespace RealFunctionCheckDerivative
{
void Check(ISingleVariableRealValuedFunction &fie, double x, double delx, bool isLinear = false);
void Check(const IRealFunction &system, std::span<const double> x, std::span<const double> delx);
} // namespace RealFunctionCheckDerivative
