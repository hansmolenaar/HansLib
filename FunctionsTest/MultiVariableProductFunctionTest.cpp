#include <gtest/gtest.h>

#include "MultiVariableProductFunction.h"
#include "MultiVariableMonomial.h"
#include "IRealFunctionUtils.h"

#include <array>

namespace {
	const double c_eps = 1.0e-12;
}

TEST(MultiVariableProductFunctionTest, Test1)
{
	auto x2 = MultiVariableMonomial({ 2 });
	MultiVariableProductFunction fie({ &x2, &x2 });  // x^4

	ASSERT_FALSE(fie.DerivativeAlwaysZero(0, 0));

	std::array<double, 1> x = { 2 };
	ASSERT_NEAR(fie.Evaluate(x), 16.0, c_eps);

	double y[] = { 0 };
	fie.IMultiVariableRealValuedFunction::Evaluate(x, y);
	ASSERT_NEAR(y[0], 16.0, c_eps);

	std::vector<double> deriv(1);
	Derivative(fie, x, deriv);
	ASSERT_NEAR(deriv[0], 32.0, c_eps);

	std::array<double, 1> delx = { 0.01 };
	CheckDerivatives(fie, x, delx);
}


TEST(MultiVariableProductFunctionTest, Test2)
{
	auto xy = MultiVariableMonomial({ 1,1 });
	auto y = MultiVariableMonomial({ 0,1 });
	MultiVariableProductFunction fie({ &xy, &y });  // x*y^2

	ASSERT_FALSE(fie.DerivativeAlwaysZero(0, 0));
	ASSERT_FALSE(fie.DerivativeAlwaysZero(0, 1));

	std::array<double, 2> x = { 2,3 };
	ASSERT_NEAR(fie.Evaluate(x), 18.0, c_eps);

	std::vector<double> deriv(2);
	Derivative(fie, x, deriv);
	ASSERT_NEAR(deriv[0], 9.0, c_eps);
	ASSERT_NEAR(deriv[1], 12.0, c_eps);

	std::array<double, 2> delx = { 0.01, 0.01 };
	CheckDerivatives(fie, x, delx);
}

TEST(MultiVariableProductFunctionTest, Test3)
{
	auto xz = MultiVariableMonomial({ 0,0,1 });
	auto xz2 = MultiVariableMonomial({ 1,0,2 });
	MultiVariableProductFunction fie({ &xz, &xz2 });  // x*z^3

	ASSERT_FALSE(fie.DerivativeAlwaysZero(0, 0));
	ASSERT_TRUE(fie.DerivativeAlwaysZero(0, 1));
	ASSERT_FALSE(fie.DerivativeAlwaysZero(0, 2));

	std::array<double, 3> x = { 3,4,2 };
	ASSERT_NEAR(fie.Evaluate(x), 24.0, c_eps);

	std::vector<double> deriv(3);
	Derivative(fie, x, deriv);
	ASSERT_NEAR(deriv[0], 8.0, c_eps);
	ASSERT_NEAR(deriv[1], 0.0, c_eps);
	ASSERT_NEAR(deriv[2], 36.0, c_eps);

	std::array<double, 3> delx = { 0.01, 0.01, 0.01 };
	CheckDerivatives(fie, x, delx);
}

TEST(MultiVariableProductFunctionTest, Test4)
{
	auto x2 = MultiVariableMonomial({ 2,0 });
	auto xy = MultiVariableMonomial({ 1,1 });
	auto y3 = MultiVariableMonomial({ 0,3 });
	MultiVariableProductFunction fie({ &x2, &xy, &y3 });  // x^3 * y^4

	ASSERT_FALSE(fie.DerivativeAlwaysZero(0, 0));
	ASSERT_FALSE(fie.DerivativeAlwaysZero(0, 1));

	std::array<double, 2> x = { 3,2 };
	ASSERT_NEAR(fie.Evaluate(x), 27 * 16, c_eps);

	std::vector<double> deriv(2);
	Derivative(fie, x, deriv);
	ASSERT_NEAR(deriv[0], 3 * 9 * 16, c_eps);
	ASSERT_NEAR(deriv[1], 27 * 4 * 8, c_eps);

	std::array<double, 2> delx = { 0.01, 0.01 };
	CheckDerivatives(fie, x, delx);
}

TEST(MultiVariableProductFunctionTest, Test5)
{
	auto x2 = MultiVariableMonomial({ 2,0 });
	MultiVariableProductFunction fie({ &x2, &x2, &x2 });  // x^6

	ASSERT_FALSE(fie.DerivativeAlwaysZero(0, 0));
	ASSERT_TRUE(fie.DerivativeAlwaysZero(0, 1));

	std::array<double, 2> x = { 2,2 };
	ASSERT_NEAR(fie.Evaluate(x), 64, c_eps);

	std::vector<double> deriv(2);
	Derivative(fie, x, deriv);
	ASSERT_NEAR(deriv[0], 6 * 32, c_eps);
	ASSERT_NEAR(deriv[1], 0.0, c_eps);

	std::array<double, 2> delx = { 0.01 , 0.1 };
	CheckDerivatives(fie, x, delx);
}






