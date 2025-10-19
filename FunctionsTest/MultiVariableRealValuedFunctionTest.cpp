#include <gtest/gtest.h>

#include "MultiVariableRealValuedFunction.h"
#include "IRealFunctionUtils.h"
#include "MultiVariableMonomial.h"
#include "RealFunctionCheckDerivative.h"

#include <array>

namespace {
	const double c_eps = 1.0e-12;
}

TEST(MultiVariableRealValuedFunctionTest, TestNull)
{
	auto fie = MultiVariableRealValuedFunction::CreateNull(2);
	ASSERT_EQ(2, fie.GetDomainDimension());
	ASSERT_EQ(1, fie.GetRangeDimension());

	ASSERT_TRUE(fie.DerivativeAlwaysZero(0,0));
	ASSERT_TRUE(fie.DerivativeAlwaysZero(0,1));

	double x[] = { 2, 3 };
	ASSERT_EQ(Evaluate(fie,x), 0.0);

	double deriv[] = {0,0};
	Derivative(fie, x, deriv);
	ASSERT_EQ(deriv[0], 0.0);
	ASSERT_EQ(deriv[1], 0.0);

	std::array<double, 2> delx = { 0.01, 0.01 };
	RealFunctionCheckDerivative::Check(fie, x, delx);
}


TEST(MultiVariableRealValuedFunctionTest, TestDivide)
{
	const MultiVariableMonomial g({ 1, 0, 0 });
	const MultiVariableMonomial h({ 0, 1, 0 });

	const auto f = g / h;

	ASSERT_FALSE(f.DerivativeAlwaysZero(0));
	ASSERT_FALSE(f.DerivativeAlwaysZero(1));
	ASSERT_TRUE(f.DerivativeAlwaysZero(2));

	double xyz[] = { 2,3,0 };
	double deriv[] = { 1,1,1 };

	double eval = Evaluate(f, xyz);
	ASSERT_NEAR(eval, 2.0 / 3.0, c_eps);

	Derivative(f, xyz, deriv);
	ASSERT_NEAR(deriv[0], 1.0 / 3.0, c_eps);
	ASSERT_NEAR(deriv[1], -2.0 / 9.0, c_eps);
	ASSERT_NEAR(deriv[2], 0.0, c_eps);
}




TEST(MultiVariableRealValuedFunctionTest, TestSubtract)
{
	const MultiVariableMonomial g({ 1, 0,0 });
	const MultiVariableMonomial h({ 0, 2,0 });

	const auto f = g - h;

	ASSERT_FALSE(f.DerivativeAlwaysZero(0));
	ASSERT_FALSE(f.DerivativeAlwaysZero(1));
	ASSERT_TRUE(f.DerivativeAlwaysZero(2));

	double xyz[] = { 2,3,4 };
	double deriv[] = { 1,1,1 };

	double eval = Evaluate(f, xyz);
	ASSERT_NEAR(eval, -7.0, c_eps);

	Derivative(f, xyz, deriv);
	ASSERT_NEAR(deriv[0], 1.0, c_eps);
	ASSERT_NEAR(deriv[1], -6.0, c_eps);
	ASSERT_NEAR(deriv[2], 0.0, c_eps);
}

