#include <gtest/gtest.h>

#include "Functions/MultiVariableFunctionEvaluateFrom01.h"
#include "Functions/MultiVariableFunctionExamples.h"
#include "Utilities/Functors.h"
#include <array>

TEST(MultiVariableFunctionEvaluateFrom01Test, xy)
{
   // 2 * x^2 * y^3
   auto fie = MultiVariableFunctionExamples::GetPolynomial(std::vector< std::pair<std::vector<int>, double>>{ {std::make_pair(std::vector<int>{2, 3}, 2.0)}});
   const MultiVariableFunctionEvaluateFrom01 scaled(std::move(fie), std::vector<Interval<double>>{Interval<double>(-1, 3), Interval<double>(-2, 2)});
   const Functors::AreClose areClose;

   std::array<double, 2> xy{ 0.5, 0.6 };
   ASSERT_TRUE(areClose(scaled(std::vector<double>{0.25, 1}), 0.0));
   ASSERT_TRUE(areClose(scaled(std::vector<double>{0.2, 0.5}), 0.0));
   ASSERT_TRUE(areClose(scaled(std::vector<double>{0, 0}), -16.0));
   ASSERT_TRUE(areClose(scaled(std::vector<double>{0, 1}), 16.0));
   ASSERT_TRUE(areClose(scaled(std::vector<double>{1, 0}), -144.0));
   ASSERT_TRUE(areClose(scaled(std::vector<double>{1, 1}), 144.0));
   ASSERT_THROW(scaled(std::vector<double>{1, -1}), MyException);
}
