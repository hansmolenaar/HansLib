#include <gtest/gtest.h>

#include "Interpolation/HierApproximation.h"
#include "Interpolation/HierBasisFunction1D_HomogenousBC.h"
#include "Utilities/MyException.h"
#include "Functions/IMultiVariableRealValuedFunction.h"
#include "Utilities/Single.h"
#include "Utilities/Functors.h"
#include "Interpolation/HierBasisFunction.h"
#include "Interpolation/HierBasisFunction1D_ExtendedLevelOneBC.h"
#include "Functions/MultiVariableFunctionEvaluateFlipped.h"
#include "Functions/MultiVariableFunctionEvaluateFrom01.h"
#include "Functions/MultiVariableFunctionExamples.h"
#include "Functions/MultiVariableRealValuedFunctionNoDerivatives.h"
#include "Interval/Interval.h"
#include "Interpolation/NodeRefinePredicateFactoryByLevel.h"

#include "Utilities/Plotting.h"

#include <filesystem>
#include <fstream>

inline constexpr double Epsilon = 1.0e-10;

namespace
{
   std::unique_ptr<IMultiVariableRealValuedFunction> GetSomePolynomial()
   {
      const std::vector< std::pair<std::vector<int>, double>> terms{
         std::make_pair(std::vector<int>{1,2} , 1.0),
         std::make_pair(std::vector<int>{1,4} , -1.0),
          std::make_pair(std::vector<int>{2,2} , -1.0),
            std::make_pair(std::vector<int>{2,4} , 1.0)
      };
      return MultiVariableFunctionExamples::GetPolynomial(terms);
   }

   void TestCollocationPoints(const IMultiVariableRealValuedFunction& expect, const HierApproximation& approximation)
   {
      const Functors::AreClose close;
      for (const auto& point : approximation.getCollocationPoints())
      {
         const double expectValue = expect.Evaluate(point);
         const double actualValue = approximation(point);
         if (!close(expectValue, actualValue))
         {
            throw MyException("TestCollocationPoints");
         }

      }
   }

}

TEST(HierBasisFunctionTest, GetSomePolynomial_1)
{
   const auto fie = GetSomePolynomial();

   ASSERT_EQ(fie->GetDomainDimension(), 2);
   const Functors::AreClose areClose{ .AbsTolerance = 1.0e-10 };
   ASSERT_TRUE(areClose(fie->Evaluate(std::vector<double>{ 0.0, 0.1}), 0.0));
   ASSERT_TRUE(areClose(fie->Evaluate(std::vector<double>{ 1.0, 0.1}), 0.0));
   ASSERT_TRUE(areClose(fie->Evaluate(std::vector<double>{ 0.1, 0.0}), 0.0));
   ASSERT_TRUE(areClose(fie->Evaluate(std::vector<double>{ 0.1, 1.0}), 0.0));
   //Plotting::MdFunctionsOnUnityToFile("HierBasisFunctionTest_GetSomePolynomial_", 2, { [&fie](std::vector<double> x) {return (*fie)(x); } }, 20, std::vector<std::string>{"x", "y", "f"});

   HierBasisFunction1D_HomogenousBC_Factory factory1D;
   HierBasisFunction_Factory factory(size_t{ 2 }, &factory1D);
   NodeRefinePredicateFactoryByLevel refineOnLevel(0);
   auto approximation = HierApproximation::Create(*fie, factory, refineOnLevel);
   ASSERT_EQ(approximation->getLeafNodesRO().size(), 1);
   ASSERT_NO_THROW(TestCollocationPoints(*fie, *approximation));

   //Plotting::MdFunctionsOnUnityToFile("HierBasisFunctionTest_GetSomePolynomial_Approximation", 2, { [&approximation](std::vector<double> x) {return (*approximation)(x); } }, 20, std::vector<std::string>{"x", "y", "approx"});
}


TEST(HierBasisFunctionTest, GetSomePolynomial_2)
{
   const auto fie = GetSomePolynomial();
   HierBasisFunction1D_HomogenousBC_Factory factory1D;
   HierBasisFunction_Factory factory(size_t{ 2 }, &factory1D);
   NodeRefinePredicateFactoryByLevel refineOnLevel(1);
   auto approximation = HierApproximation::Create(*fie, factory, refineOnLevel);
   ASSERT_EQ(approximation->getLeafNodesRO().size(), 4);
   ASSERT_EQ(approximation->getAllTreeNodesRO().size(), 5);
   ASSERT_NO_THROW(TestCollocationPoints(*fie, *approximation));

   //Plotting::MdFunctionsOnUnityToFile("HierBasisFunctionTest_GetSomePolynomial_Approximation", 2, { [&approximation](std::vector<double> x) {return (*approximation)(x); } }, 20, std::vector<std::string>{"x", "y", "approx"});
}


TEST(HierBasisFunctionTest, GetSomePolynomial_3)
{
   const auto fie = GetSomePolynomial();
   HierBasisFunction1D_HomogenousBC_Factory factory1D;
   HierBasisFunction_Factory factory(size_t{ 2 }, &factory1D);
   NodeRefinePredicateFactoryByLevel refineOnLevel(2);
   auto approximation = HierApproximation::Create(*fie, factory, refineOnLevel);

   std::vector<HierMultiIndex> indices;
   str::transform(approximation->getAllTreeNodesRO(), std::back_inserter(indices), [](const HierTreeNode* hn) {return hn->getMultiIndex(); });
   str::sort(indices);
   std::string str;
   for (const auto& s : indices)
   {
      str += (s.toString() + "\n");
   }
   ASSERT_EQ(approximation->getAllTreeNodesRO().size(), 17);
   ASSERT_EQ(approximation->getLeafNodesRO().size(), 12);
   ASSERT_NO_THROW(TestCollocationPoints(*fie, *approximation));


   //Plotting::MdFunctionsOnUnityToFile("HierBasisFunctionTest_GetSomePolynomial_Approximation", 2, { [&approximation](std::vector<double> x) {return (*approximation)(x); } }, 20, std::vector<std::string>{"x", "y", "approx"});
}


TEST(HierBasisFunctionTest, Linear)
{
   const std::array< double, 2 > xin{ 0.52, 0.51 };
   // 2*x + 3*y
   const auto linear = MultiVariableFunctionExamples::GetPolynomial(std::vector< std::pair<std::vector<int>, double>>{ {std::vector<int>{1, 0}, 2.0}, { std::vector<int>{0,1}, 3.0 }});
   const double expect = linear->Evaluate(xin);

   HierBasisFunction1D_ExtendedLevelOneBC_Factory factory1D;
   HierBasisFunction_Factory factory(size_t{ 2 }, &factory1D);
   constexpr int maxLevel = 7;
   for (int level = 1; level < maxLevel; ++level)
   {
      NodeRefinePredicateFactoryByLevel refineOnLevel(level);
      auto approximation = HierApproximation::Create(*linear, factory, refineOnLevel);
      ASSERT_NO_THROW(TestCollocationPoints(*linear, *approximation));
      const double dif = std::abs((*approximation)(xin) - expect);
      const Functors::AreClose close;
      ASSERT_TRUE(close((*approximation)(xin), expect));
   }
}

TEST(HierBasisFunctionTest, TestConvergence)
{
   const std::array< double, 2 > xin{ 0.52, 0.53 };
   const auto fie = GetSomePolynomial();
   const double expect = fie->Evaluate(xin);

   HierBasisFunction1D_HomogenousBC_Factory factory1D;
   HierBasisFunction_Factory factory(size_t{ 2 }, &factory1D);
   constexpr int maxLevel = 8;
   std::vector<double> maxSurplus;
   for (int level = 2; level < maxLevel; ++level)
   {
      NodeRefinePredicateFactoryByLevel refineOnLevel(level);
      auto approximation = HierApproximation::Create(*fie, factory, refineOnLevel);
      ASSERT_NO_THROW(TestCollocationPoints(*fie, *approximation));
      maxSurplus.push_back(approximation->getMaxSurplus());
   }

   for (size_t n = 2; n < maxSurplus.size(); ++n)
   {
      ASSERT_TRUE(maxSurplus.at(n) < 0.5 * maxSurplus.at(n - 1));
   }
}


TEST(HierBasisFunctionTest, CamelBack)
{
   auto camelHump = MultiVariableFunctionExamples::SixHumpCamelFunction();
   auto flipped = std::make_unique< MultiVariableFunctionEvaluateFlipped>(std::move(camelHump));
   auto scaled = std::make_unique< MultiVariableFunctionEvaluateFrom01>(std::move(flipped), std::vector<Interval<double>>{ Interval<double>(-3, 3), Interval<double>(-2, 2)});
   auto fie = std::make_unique<MultiVariableRealValuedFunctionNoDerivatives>(2, std::move(scaled));

   HierBasisFunction1D_ExtendedLevelOneBC_Factory factory1D;
   HierBasisFunction_Factory factory(size_t{ 2 }, &factory1D);
   constexpr size_t maxLevel = 8;

   NodeRefinePredicateFactoryByLevel refineOnLevel(maxLevel);
   auto approximation = HierApproximation::Create(*fie, factory, refineOnLevel);
   const auto& nodes = approximation->getAllTreeNodesRO();
   std::vector<double> values(nodes.size());
   str::transform(nodes, values.begin(), [](const auto* hn) {return hn->Value; });
   const double maxValue = *std::max_element(values.begin(), values.end());
   ASSERT_NEAR(maxValue, 1.0, 0.02);
   //Plotting::MdFunctionsOnUnityToFile("HierBasisFunctionTest_CamelBack", 2, { [&approximation](std::vector<double> x) {return (*approximation)(x); } }, 64, std::vector<std::string>{"x", "y", "approx"});
}


TEST(HierBasisFunctionTest, SquaredHat)
{
   const size_t dimension = 10;
   std::vector<double> pos(dimension);
   str::transform(str::iota_view(size_t{ 0 }, dimension), pos.begin(), [](size_t n) {return 1.0 / std::sqrt(n + 2.0); });
   const auto squaredHat = MultiVariableFunctionExamples::SkewedHatSquared(pos);

   HierBasisFunction1D_HomogenousBC_Factory factory1D;
   HierBasisFunction_Factory factory(dimension, &factory1D);
   constexpr size_t maxLevel = 3;

   NodeRefinePredicateFactoryByLevel refineOnLevel(maxLevel);
   auto approximation = HierApproximation::Create(*squaredHat.Function, factory, refineOnLevel);
   const auto& nodes = approximation->getAllTreeNodesRO();
   std::vector<double> values(nodes.size());
   str::transform(nodes, values.begin(), [](const auto* hn) {return hn->Value; });
   const double maxValue = *std::max_element(values.begin(), values.end());
   ASSERT_EQ(nodes.size(), 2001);
   ASSERT_NEAR(maxValue, 0.72028645911812739, 1.0e-6);
   //Plotting::MdFunctionsOnUnityToFile("HierBasisFunctionTest_SquaredHat", 2, { [&squaredHat](std::vector<double> x) {return (*squaredHat.Function)(x); } }, 64, std::vector<std::string>{"x", "y", "approx"});
}
