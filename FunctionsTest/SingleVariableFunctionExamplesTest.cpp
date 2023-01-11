#include <gtest/gtest.h>

#include "Functions/SingleVariableFunctionExamples.h"
#include "Utilities/Functors.h"


TEST(SingleVariableFunctionExamplesTest, SkewedHas)
{
	const double xpeak = std::sqrt(0.5);
	const auto fiePtr = SingleVariableFunctionExamples::GetSkewedtHatFunction(xpeak);
	const auto& fie = *fiePtr;
	Functors::AreClose areClose;
	ASSERT_TRUE(areClose(fie.Evaluate(-1), 0.0));
	ASSERT_TRUE(areClose(fie.Evaluate(0), 0.0));
	ASSERT_TRUE(areClose(fie.Evaluate(xpeak/2), 0.5));
	ASSERT_TRUE(areClose(fie.Evaluate(xpeak), 1.0));
	ASSERT_TRUE(areClose(fie.Evaluate((1+xpeak) / 2), 0.5));
	ASSERT_TRUE(areClose(fie.Evaluate(1), 0.0));
	ASSERT_TRUE(areClose(fie.Evaluate(2), 0.0));
}

