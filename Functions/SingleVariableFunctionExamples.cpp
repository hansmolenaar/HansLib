#include "SingleVariableFunctionExamples.h"
#include "BoundsCheck.h"
#include "Interval.h"
#include "SingleVariableFunctionOnIntervals.h"
#include "SingleVariablePolynomial.h"
#include "SingleVariableRealValuedFunction.h"

std::shared_ptr<ISingleVariableRealValuedFunction> SingleVariableFunctionExamples::GetRungeFunction()
{
    return std::make_unique<SingleVariableRealValuedFunction>([](double x) { return 1.0 / (1.0 + 25.0 * x * x); });
}

// f(0) = 0
// f(xpeak) = 1
// f(1) = 0
std::shared_ptr<ISingleVariableRealValuedFunction> SingleVariableFunctionExamples::GetSkewedtHatFunction(double xpeak)
{
    return GetDiscontinuousHatFunction(xpeak, 1.0, 1.0);
}

std::shared_ptr<ISingleVariableRealValuedFunction> SingleVariableFunctionExamples::GetStepFunction(double xstep)
{
    BoundsCheck<double>::Create(0.0, 1.0).check(xstep);
    return std::make_unique<SingleVariableRealValuedFunction>([xstep](double x) {
        if (x < xstep)
            return 0.0;
        return 1.0;
    });
}

std::shared_ptr<ISingleVariableRealValuedFunction> SingleVariableFunctionExamples::GetDiscontinuousHatFunction(
    double xPeak, double yPeakLeft, double yPeakRight)
{
    BoundsCheck<double>::Create(0.0, 1.0).check(xPeak);

    const Interval<double> intervalL(0.0, xPeak);
    std::shared_ptr<SingleVariableFunctionOnInterval> funL =
        SingleVariableFunctionOnInterval::CreateLinearOnInterval(intervalL, 0, yPeakLeft);
    const Interval<double> intervalR(xPeak, 1.0);
    std::shared_ptr<SingleVariableFunctionOnInterval> funR =
        SingleVariableFunctionOnInterval::CreateLinearOnInterval(intervalR, yPeakRight, 0);

    return std::make_shared<SingleVariableFunctionOnIntervals>(
        std::vector<std::shared_ptr<SingleVariableFunctionOnInterval>>{funL, funR}, 0.0);
}