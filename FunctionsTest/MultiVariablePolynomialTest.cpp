#include <gtest/gtest.h>

#include "ISingleVariableRealValuedFunction.h"
#include "MultiVariablePolynomial.h"
#include "IRealFunctionUtils.h"
#include "RealFunctionCheckDerivative.h"

#include <array>

TEST(MultiVariablePolynomialTest, Test0)
{
	MultiVariablePolynomial fie(2);
	ASSERT_EQ(fie.GetDomainDimension(), 2);
	ASSERT_TRUE(fie.DerivativeAlwaysZero(0,0));
	ASSERT_TRUE(fie.DerivativeAlwaysZero(0,1));

	ASSERT_ANY_THROW(fie.DerivativeAlwaysZero(1, 0));

	std::array<double, 2> x = { 3.0, 4.0 };
	std::array<double, 2> deriv;

	ASSERT_EQ(Evaluate(fie,x), 0.0);
	Derivative(fie,x, deriv);
	ASSERT_EQ(deriv[0], 0.0);
	ASSERT_EQ(deriv[1], 0.0);

	RealFunctionCheckDerivative::Check(fie, x, x);
}


TEST(MultiVariablePolynomialTest, Test1)
{
	MultiVariablePolynomial fie(2);
	std::vector<int> powers = { 0,2 };
	fie.Add(2.0, powers);

	ASSERT_EQ(fie.GetDomainDimension(), 2);
	ASSERT_TRUE(fie.DerivativeAlwaysZero(0,0));
	ASSERT_FALSE(fie.DerivativeAlwaysZero(0,1));

	std::array<double, 2> x = { 2.0, 3.0 };
	std::array<double, 2> deriv;

	ASSERT_EQ(Evaluate(fie, x), 18.0);
	Derivative(fie, x, deriv);
	ASSERT_EQ(deriv[0], 0.0);
	ASSERT_EQ(deriv[1], 12.0);

	std::array<double, 2> delx = { 0.01, 0.01 };
	RealFunctionCheckDerivative::Check(fie, x, delx);
}


TEST(MultiVariablePolynomialTest, Test2)
{
	MultiVariablePolynomial fie(2);
	std::vector<int> powers = { 2,0 };
	fie.Add(1.0, powers);
	powers = { 1,1 };
	fie.Add(2.0, powers);

	ASSERT_EQ(fie.GetDomainDimension(), 2);
	ASSERT_FALSE(fie.DerivativeAlwaysZero(0,0));
	ASSERT_FALSE(fie.DerivativeAlwaysZero(0,1));

	std::array<double, 2> x = { 2.0, 3.0 };
	std::array<double, 2> deriv;

	auto tmp = Evaluate(fie, x);
	ASSERT_EQ(Evaluate(fie, x), 16.0);
	Derivative(fie, x, deriv);
	ASSERT_EQ(deriv[0], 10.0);
	ASSERT_EQ(deriv[1], 4.0);

	RealFunctionCheckDerivative::Check(fie, x, x);
}








