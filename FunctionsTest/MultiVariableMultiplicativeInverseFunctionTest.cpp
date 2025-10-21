#include <gtest/gtest.h>

#include "IRealFunctionUtils.h"
#include "MultiVariableMonomial.h"
#include "MultiVariableMultiplicativeInverseFunction.h"
#include "RealFunctionCheckDerivative.h"

#include <array>

TEST(MultiVariableMultiplicativeInverseFunctionTest, Test1)
{
    auto fie = MultiVariableMonomial({0, 2});
    auto inv = MultiVariableMultiplicativeInverseFunction(fie);
    ASSERT_TRUE(inv.DerivativeAlwaysZero(0));
    ASSERT_FALSE(inv.DerivativeAlwaysZero(1));

    std::array<double, 2> x = {3, 2};
    ASSERT_EQ(inv.Evaluate(x), 0.25);

    std::vector<double> deriv(2);
    Derivative(inv, x, deriv);
    ASSERT_EQ(deriv[0], 0.0);
    ASSERT_EQ(deriv[1], -0.25);

    std::array<double, 2> delx = {0.01, 0.01};
    RealFunctionCheckDerivative::Check(inv, x, delx);
}
