#include <gtest/gtest.h>

#include "Functions/MultiVariableFunctionExamples.h"
#include "Utilities/Functors.h"
#include "Utilities/Plotting.h"

#include <array>

TEST(MultiVariableFunctionExamplesTest, SixHumpCamelFunction)
{
   auto fie = MultiVariableFunctionExamples::SixHumpCamelFunction();
   const double minimum = -1.0316284535;

   std::array<double, 2> x = { 0.0898,-0.7126 };
   ASSERT_NEAR(fie->Evaluate(x), minimum, 1.0e-6);

   x = { -0.0898, 0.7126 };
   ASSERT_NEAR(fie->Evaluate(x), minimum, 1.0e-6);
}


TEST(MultiVariableFunctionExamplesTest, DiscontinuousHatProduct)
{
   const auto fie = MultiVariableFunctionExamples::DiscontinuousHatProduct(std::vector<double>{0.4, 0.6}, std::vector<double>{2, 1}, std::vector<double> {1, 3});
   Functors::AreClose areClose;
   ASSERT_TRUE(areClose( fie.Maximum->Extremum, 6.0));
   ASSERT_TRUE(areClose(fie.Maximum->Positions.at(0).at(0), 0.4));
   ASSERT_TRUE(areClose(fie.Maximum->Positions.at(0).at(1), 0.6));
   Plotting::MdFunctionsOnUnityToFile("DiscontinuousHatProduct", 2, { [&fie](std::vector<double> x) {return (*fie.Function)(x); } }, 64, std::vector<std::string>{"x", "y", "approx"});
}
