#include <gtest/gtest.h>

#include "CompassSearch.h"
#include "Functors.h"
#include "MultiVariableFunctionExamples.h"
#include "MultiVariableRealValuedFunctionEvaluateCached.h"
#include "Single.h"
#include "SingleVariablePolynomial.h"

TEST(CompassSearchTest, SingleArgument)
{
    std::shared_ptr<IMultiVariableFunctionEvaluate> fie = MultiVariableFunctionExamples::GetPolynomial(
        std::vector<std::pair<std::vector<int>, double>>{{std::vector<int>{2}, 1.0}});
    CompassSearch cs(std::move(fie), std::vector<double>{0.9}, 1.0);
    auto retval = cs.iterate();
    ASSERT_EQ(retval.Status, StepSucces);
    constexpr Functors::AreClose areClose;
    ASSERT_TRUE(areClose(Utilities::Single(retval.ArgMinimum), -0.1));
    ASSERT_TRUE(areClose(retval.EvalMinimum, 0.01));
}

TEST(CompassSearchTest, CompassSearch_TwoArguments)
{
    const std::vector<double> roots{0.6, 0.7};
    const std::vector<double> init{0.5, 0.5};
    std::vector<std::shared_ptr<ISingleVariableRealValuedFunction>> fies;
    for (double root : roots)
    {
        const std::vector<std::tuple<double, int>> terms{std::make_tuple(root, 0), std::make_tuple(-1.0, 1)};
        fies.emplace_back(std::make_unique<SingleVariablePolynomial>(SingleVariablePolynomial::Create(terms)));
    }

    auto soq = MultiVariableFunctionExamples::SumOfSquares(fies);
    auto fie = std::make_shared<MultiVariableRealValuedFunctionEvaluateCached>(soq);
    CompassSearch cs(std::move(fie), init, 0.3);
    const auto &objectiveFunctions =
        dynamic_cast<const MultiVariableRealValuedFunctionEvaluateCached &>(cs.getObjectiveFunction());

    constexpr int maxLoop = 10;
    IterativeMinimizationStep interationResult;
    for (int loop = 0; loop < maxLoop; ++loop)
    {
        interationResult = cs.iterate();
        ASSERT_EQ(interationResult.Status, StepSucces);
    }

    constexpr Functors::AreClose areClose;
    ASSERT_EQ(objectiveFunctions.getDimension(), 2);
    ASSERT_EQ(objectiveFunctions.getNumEvaluations(), 32);
    ASSERT_TRUE(areClose(4.8828125000001392e-05, interationResult.EvalMinimum));
}
