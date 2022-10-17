#include <gtest/gtest.h>

#include "Functions/ISingleVariableRealValuedFunctionUtils.h"
#include "Functions/ISingleVariableRealValuedFunction.h"
#include "Functions/SingleVariablePolynomial.h"


TEST(SingleVariablePolynomial, Test0)
{
	SingleVariablePolynomial fie;
	ASSERT_FALSE(fie.IsNonConstant());
	ASSERT_EQ( fie.Evaluate(2.0), 0.0);
	ASSERT_EQ(fie.Derivative(2.0),  0.0);

	fie.Add(3.0, 2);
	ASSERT_TRUE(fie.IsNonConstant());
	ASSERT_EQ(fie.Evaluate(1.0), 3.0);
	ASSERT_EQ(fie.Derivative(1.0), 6.0);
	CheckDerivative(fie, 1.0, 0.1);

	fie.Add(2.0, 1);
	ASSERT_EQ(fie.Evaluate(1.0), 5.0);
	ASSERT_EQ(fie.Derivative(1.0), 8.0);
	CheckDerivative(fie, 1.0, 0.1);
}







