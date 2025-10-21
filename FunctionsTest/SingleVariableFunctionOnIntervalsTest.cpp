#include <gtest/gtest.h>

#include "Functors.h"
#include "SingleVariableFunctionOnIntervals.h"
#include "SingleVariableMonomial.h"

TEST(SingleVariableFunctionsOnInterval, Simple)
{
    constexpr Functors::AreClose areClose;
    const Interval<double> domain1(-1, 1);
    const Interval<double> domain2(2, 3);
    auto monomial1 = std::make_shared<SingleVariableMonomial>(1);
    auto monomial2 = std::make_shared<SingleVariableMonomial>(2);
    auto function1 = std::make_shared<SingleVariableFunctionOnInterval>(monomial1, domain1);
    auto function2 = std::make_shared<SingleVariableFunctionOnInterval>(monomial2, domain2);
    const SingleVariableFunctionOnIntervals function(
        std::vector<std::shared_ptr<SingleVariableFunctionOnInterval>>{function1, function2}, 999.0);
    ASSERT_FALSE(function.IsNonConstant());
    ASSERT_FALSE(function.HasDerivative());
    ASSERT_THROW(function.Derivative(3.0), MyException);

    ASSERT_TRUE(areClose(function(0.5), 0.5));
    ASSERT_TRUE(areClose(function(1.5), 999.0));
    ASSERT_TRUE(areClose(function(2.5), 6.25));
}

TEST(SingleVariableFunctionsOnInterval, exception)
{
    constexpr Functors::AreClose areClose;
    const Interval<double> domain1(-1, 1);
    auto monomial1 = std::make_shared<SingleVariableMonomial>(1);
    auto function1 = std::make_shared<SingleVariableFunctionOnInterval>(monomial1, domain1);
    const SingleVariableFunctionOnIntervals function(
        std::vector<std::shared_ptr<SingleVariableFunctionOnInterval>>{function1});
    ASSERT_THROW(function(3.0), MyException);
}
