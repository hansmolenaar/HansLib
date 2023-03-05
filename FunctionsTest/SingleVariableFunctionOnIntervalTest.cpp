#include <gtest/gtest.h>

#include "SingleVariableFunctionOnInterval.h"
#include "SingleVariableMonomial.h"


TEST(SingleVariableFunctionOnInterval, Simple)
{
	const Interval<double> domain(-2, 2);
	auto monomial = std::make_shared<SingleVariableMonomial>(2);
	const SingleVariableFunctionOnInterval function(monomial, domain);
	ASSERT_TRUE(function.IsNonConstant());
	ASSERT_EQ(function.Evaluate(0.5), 0.25);
	ASSERT_EQ(function.Derivative(0.5), 1.0);

	ASSERT_THROW(function.Evaluate(3.0), MyException);
}







