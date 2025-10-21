#pragma once

#include "IMultiVariableFunctionEvaluate.h"
#include "IMultiVariableRealValuedFunction.h"
#include "ISingleVariableRealValuedFunction.h"
#include "Interval.h"
#include "MultiVariableRealValuedFunctionEvaluateCached.h"
#include <functional>
#include <limits>
#include <memory>
#include <vector>

namespace MultiVariableFunctionExamples
{
struct ExampleExtremumInfo
{
    double Extremum = std::numeric_limits<double>::quiet_NaN();
    std::vector<std::vector<double>> Positions;
};

struct Examplefunction
{
    std::shared_ptr<MultiVariableRealValuedFunctionEvaluateCached> Function;
    std::vector<Interval<double>> Domain;
    std::unique_ptr<ExampleExtremumInfo> Minimum;
    std::unique_ptr<ExampleExtremumInfo> Maximum;
};

std::shared_ptr<IMultiVariableRealValuedFunction> GetPolynomial(
    const std::vector<std::pair<std::vector<int>, double>> &);
std::shared_ptr<IMultiVariableRealValuedFunction> SixHumpCamelFunction();
std::shared_ptr<IMultiVariableFunctionEvaluate> SumOfSquares(
    std::vector<std::shared_ptr<ISingleVariableRealValuedFunction>> &functions);

Examplefunction SkewedHatSquared(const std::vector<double> &pos);
Examplefunction DiscontinuousHatProduct(const std::vector<double> &xpeak, const std::vector<double> &ypeakLeft,
                                        const std::vector<double> &ypeakRight);
Examplefunction SumOfSquares(int dim);

} // namespace MultiVariableFunctionExamples
