#include <gtest/gtest.h>

#include "Functions/ISingleVariableRealValuedFunctionUtils.h"
#include "Functions/ISingleVariableRealValuedFunction.h"
#include "Functions/SingleVariableMonomial.h"


TEST(SingleVariableMonomialTest, Test0)
{
	auto fie = SingleVariableMonomial(0);
	ASSERT_EQ(fie.GetPower(), 0);
	ASSERT_FALSE(fie.IsNonConstant());
	ASSERT_EQ( fie.Evaluate(2.0), 1.0);
	ASSERT_EQ(fie.Derivative(2.0),  0.0);
}


TEST(SingleVariableMonomialTest, Test1)
{
	auto fie = SingleVariableMonomial(1);
	ASSERT_EQ(fie.GetPower(), 1);
	ASSERT_TRUE(fie.IsNonConstant());
	ASSERT_EQ(fie.Evaluate(2.0), 2.0);
	ASSERT_EQ(fie.Derivative(2.0), 1.0);
	ISingleVariableRealValuedFunctionUtils::CheckDerivative(fie, 1.0, 0.1, true);
}


TEST(SingleVariableMonomialTest, Test2)
{
	auto fie = SingleVariableMonomial(2);
	ASSERT_TRUE(fie.IsNonConstant());
	ASSERT_EQ(fie.GetPower(), 2);
	ASSERT_EQ(fie.Evaluate(3.0), 9.0);
	ASSERT_EQ(fie.Derivative(3.0), 6.0);
	ISingleVariableRealValuedFunctionUtils::CheckDerivative(fie, 1.0, 0.1);
}

TEST(SingleVariableMonomialTest, TestError)
{
	ASSERT_ANY_THROW(SingleVariableMonomial(-1));
}




