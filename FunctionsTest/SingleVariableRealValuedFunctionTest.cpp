#include <gtest/gtest.h>

#include "SingleVariableRealValuedFunction.h"



TEST(SingleVariableRealValuedFunctionTest, Test0)
{
	const double eps = 1.0e-12;
	auto fie = SingleVariableRealValuedFunction([](double x) {return x * x; }, [](double x) {return 2 * x; });
	ASSERT_TRUE(fie.IsNonConstant());
	ASSERT_NEAR(fie.Evaluate(3.0), 9.0, eps);
	ASSERT_NEAR(fie.Derivative(3.0), 6.0, eps);
}

