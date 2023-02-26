#include <gtest/gtest.h>

#include "MultiVariableMonomial.h"
#include "IRealFunctionUtils.h"

#include <array>

TEST(MultiVariableMonomialTest, Test1)
{
	auto fie = MultiVariableMonomial({ 0,2 });
	ASSERT_TRUE(fie.DerivativeAlwaysZero(0,0));
	ASSERT_FALSE(fie.DerivativeAlwaysZero(0,1));

	std::array<double, 2> x = { 2, 3 };
	ASSERT_EQ(fie.Evaluate(x), 9.0);

	double y[] = {0};
	fie.IMultiVariableRealValuedFunction::Evaluate(x, y);
	ASSERT_EQ(y[0], 9.0);

	std::vector<double> deriv(2);
	Derivative(fie, x, deriv);
	ASSERT_EQ(deriv[0], 0.0);
	ASSERT_EQ(deriv[1], 6.0);

	std::array<double, 2> delx = { 0.01, 0.01 };
	CheckDerivatives(fie, x, delx);
}


TEST(MultiVariableMonomialTest, Test2)
{
	auto fie = MultiVariableMonomial({2,3,4});

	std::array<double, 3> x = { 1, 1, 1 };
	ASSERT_EQ(fie.Evaluate(x), 1.0);
	std::vector<double> deriv(3);
	Derivative(fie, x, deriv);
	ASSERT_EQ(deriv[0], 2.0);
	ASSERT_EQ(deriv[1], 3.0);
	ASSERT_EQ(deriv[2], 4.0);

	x = { 2, 2, 2 };
	ASSERT_EQ(fie.Evaluate(x), 512);
	Derivative(fie, x, deriv);
	ASSERT_EQ(deriv[0], 2 * 256);
	ASSERT_EQ(deriv[1], 3 * 256);
	ASSERT_EQ(deriv[2], 4 * 256);
}


TEST(MultiVariableMonomialTest, Test3)
{
	auto fie = MultiVariableMonomial({1,2});

	std::array<double, 2> x = { 3,2 };
	ASSERT_EQ(fie.Evaluate(x), 12.0);
	std::vector<double> deriv(2);
	Derivative(fie, x, deriv);
	ASSERT_EQ(deriv[0], 4.0);
	ASSERT_EQ(deriv[1], 12.0);
}


TEST(MultiVariableMonomialTest, TestError)
{
	ASSERT_ANY_THROW(auto mon = MultiVariableMonomial({}); );

	auto fie = MultiVariableMonomial({2,0,4});

	ASSERT_ANY_THROW(fie.DerivativeAlwaysZero(-1,0));
	ASSERT_ANY_THROW(fie.DerivativeAlwaysZero(3,0));
	ASSERT_ANY_THROW(fie.DerivativeAlwaysZero(1, 0));
}





