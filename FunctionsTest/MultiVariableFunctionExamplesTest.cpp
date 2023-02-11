#include <gtest/gtest.h>

#include "Functions/MultiVariableFunctionExamples.h"

#include <array>

TEST(MultiVariableFunctionExamplesTest, SixHumpCamelFunction)
{
	auto fie = MultiVariableFunctionExamples:: SixHumpCamelFunction();
	const double minimum = -1.0316284535;

	std::array<double, 2> x = { 0.0898,-0.7126 };
	ASSERT_NEAR(fie->Evaluate(x), minimum, 1.0e-6);

	 x = { -0.0898, 0.7126 };
	ASSERT_NEAR(fie->Evaluate(x), minimum, 1.0e-6);
}
