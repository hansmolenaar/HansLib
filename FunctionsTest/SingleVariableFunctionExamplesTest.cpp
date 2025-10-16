#include <gtest/gtest.h>

#include "SingleVariableFunctionExamples.h"
#include "Functors.h"

#include <cmath>

TEST(SingleVariableFunctionExamplesTest, SkewedHat)
{
   const double xpeak = std::sqrt(0.5);
   const auto fiePtr = SingleVariableFunctionExamples::GetSkewedtHatFunction(xpeak);
   const auto& fie = *fiePtr;
   Functors::AreClose areClose;
   ASSERT_TRUE(areClose(fie.Evaluate(-1), 0.0));
   ASSERT_TRUE(areClose(fie.Evaluate(0), 0.0));
   ASSERT_TRUE(areClose(fie.Evaluate(xpeak / 2), 0.5));
   ASSERT_TRUE(areClose(fie.Evaluate(xpeak), 1.0));
   ASSERT_TRUE(areClose(fie.Evaluate((1 + xpeak) / 2), 0.5));
   ASSERT_TRUE(areClose(fie.Evaluate(1), 0.0));
   ASSERT_TRUE(areClose(fie.Evaluate(2), 0.0));
}


TEST(SingleVariableFunctionExamplesTest, DiscontinuousHatFunction)
{
   constexpr double delta = 1.0e-10;
   const double xpeak = std::sqrt(0.5);
   const auto fiePtr = SingleVariableFunctionExamples::GetDiscontinuousHatFunction(xpeak, 2, 1);
   const auto& fie = *fiePtr;
   Functors::AreClose areClose{ 1.0e-8 };
   ASSERT_TRUE(areClose(fie.Evaluate(-1), 0.0));
   ASSERT_TRUE(areClose(fie.Evaluate(0), 0.0));
   ASSERT_TRUE(areClose(fie.Evaluate(xpeak / 2), 1.0));
   ASSERT_TRUE(areClose(fie.Evaluate(xpeak - delta), 2.0));
   ASSERT_TRUE(areClose(fie.Evaluate(xpeak + delta), 1.0));
   ASSERT_TRUE(areClose(fie.Evaluate((1 + xpeak) / 2), 0.5));
   ASSERT_TRUE(areClose(fie.Evaluate(1), 0.0));
   ASSERT_TRUE(areClose(fie.Evaluate(2), 0.0));
}

