#include <gtest/gtest.h>

#include "MultiVariableFunctionExamples.h"
#include "Functors.h"
#include "Plotting.h"
#include "SingleVariableMonomial.h"

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
   ASSERT_TRUE(areClose(fie.Maximum->Extremum, 6.0));
   ASSERT_TRUE(areClose(fie.Maximum->Positions.at(0).at(0), 0.4));
   ASSERT_TRUE(areClose(fie.Maximum->Positions.at(0).at(1), 0.6));
   //Plotting::MdFunctionsOnUnityToFile("DiscontinuousHatProduct", 2, { [&fie](std::vector<double> x) {return (*fie.Function)(x); } }, 64, std::vector<std::string>{"x", "y", "approx"});
}



TEST(MultiVariableFunctionExamplesTest, SumOfSquares)
{
   const auto fie = MultiVariableFunctionExamples::SumOfSquares(2);
   Functors::AreClose areClose;
   ASSERT_TRUE(areClose(fie.Minimum->Extremum, 0.0));
   ASSERT_TRUE(areClose(fie.Minimum->Positions.at(0).at(0), 0.0));
   ASSERT_TRUE(areClose(fie.Minimum->Positions.at(0).at(1), 0.0));

   ASSERT_TRUE(areClose((*fie.Function)(std::vector<double>{1, 2}), 5.0));
   ASSERT_EQ(fie.Function->getNumEvaluations(), 1);
   //Plotting::MdFunctionsOnUnityToFile("SumOfSquares", 2, { [&fie](std::vector<double> x) {return (*fie.Function)(x); } }, 64, std::vector<std::string>{"x", "y", "approx"});
}


TEST(MultiVariableFunctionExamplesTest, SumOfSquares2)
{
   std::vector<std::shared_ptr<ISingleVariableRealValuedFunction>> fies;
   fies.emplace_back(std::make_shared<SingleVariableMonomial>(1));
   fies.emplace_back(std::make_shared<SingleVariableMonomial>(1));
   auto fie = MultiVariableFunctionExamples::SumOfSquares(fies);
   
   Functors::AreClose areClose;
   const double eval = (*fie)(std::vector<double>{ 1.0 / 2.0, 1.0 / 3.0});
   ASSERT_TRUE(areClose(eval, 13.0/36.0));
   Plotting::MdFunctionsOnUnityToFile("SumOfSquares2", 2, { [&fie](std::vector<double> x) {return (*fie)(x); } }, 64, std::vector<std::string>{"x", "y", "approx"});
}