#include <gtest/gtest.h>

#include "MultiVariableVectorComponentFunction.h"



TEST(MultiVariableVectorComponentFunctionTest, Test1)
{
	const int ind = 1;
	const int dim = 3;
	auto fie = MultiVariableVectorComponentFunction::Instance(ind, dim);

	ASSERT_EQ(dim, fie.GetDomainDimension());
	ASSERT_EQ(1, fie.GetRangeDimension());

	ASSERT_TRUE(fie.DerivativeAlwaysZero(0));
	ASSERT_FALSE(fie.DerivativeAlwaysZero(1));
	ASSERT_TRUE(fie.DerivativeAlwaysZero(2));

	double x[dim] = { 1,2, 3 };
	auto eval = fie.Evaluate(x);
	ASSERT_EQ(2.0, eval);


	double deriv[dim];
	fie.Derivative(x, deriv);
	ASSERT_EQ(0.0, deriv[0]);
	ASSERT_EQ(1.0, deriv[1]);
	ASSERT_EQ(0.0, deriv[2]);
}








