#include <gtest/gtest.h>


#include "Functions/MultiVariableFunctionEvaluateFlipped.h"
#include "Functions//MultiVariableFunctionExamples.h"
#include "Functors.h"
#include <array>

TEST(MultiVariableFunctionEvaluateFlippedTest, xy)
{
   // x * y
   auto fie = MultiVariableFunctionExamples::GetPolynomial(std::vector< std::pair<std::vector<int>, double>>{ {std::make_pair(std::vector<int>{1, 1}, 1.0)}});
   const MultiVariableFunctionEvaluateFlipped flipped(std::move(fie));
   const Functors::AreClose areClose;

   ASSERT_EQ(flipped.getDimension(), 2);
   ASSERT_TRUE(areClose(flipped(std::vector<double>{2, 3}), -6.0));
}
