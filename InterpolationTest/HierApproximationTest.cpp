#include <gtest/gtest.h>

#include "Interpolation/HierApproximation.h"
#include "Interpolation/HierBasisFunction1D_HomogenousBC.h"
#include "Utilities/MyException.h"
#include "Functions/IMultiVariableRealValuedFunction.h"
#include "Utilities/Single.h"
#include "Utilities/Functors.h"
#include "Interpolation/HierBasisFunction.h"

#include "Functions/MultiVariableFunctionExamples.h"

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

   const HierBasisFunction1D_HomogenousBC_Factory factory1D;
   HierBasisFunction_Factory factory(size_t{ 2 }, &factory1D);
   const HierApproximation::RefineInAllDirectionsOnL1Norm refineOnNorm{ 2 };
   auto approximation = HierApproximation::Create(*fie, factory, refineOnNorm);
   ASSERT_EQ(approximation->getLeafNodesRO().size(), 1);
   ASSERT_NO_THROW(TestCollocationPoints(*fie, *approximation));

   //Plotting::MdFunctionsOnUnityToFile("HierBasisFunctionTest_GetSomePolynomial_Approximation", 2, { [&approximation](std::vector<double> x) {return (*approximation)(x); } }, 20, std::vector<std::string>{"x", "y", "approx"});
}


TEST(HierBasisFunctionTest, GetSomePolynomial_2)
{
   const auto fie = GetSomePolynomial();
   const HierBasisFunction1D_HomogenousBC_Factory factory1D;
   HierBasisFunction_Factory factory(size_t{ 2 }, &factory1D);
   const HierApproximation::RefineInAllDirectionsOnL1Norm refineOnNorm{ 3 };
   auto approximation = HierApproximation::Create(*fie, factory, refineOnNorm);
   ASSERT_EQ(approximation->getLeafNodesRO().size(), 4);
   ASSERT_EQ(approximation->getAllTreeNodesRO().size(), 5);
   ASSERT_NO_THROW(TestCollocationPoints(*fie, *approximation));

   //Plotting::MdFunctionsOnUnityToFile("HierBasisFunctionTest_GetSomePolynomial_Approximation", 2, { [&approximation](std::vector<double> x) {return (*approximation)(x); } }, 20, std::vector<std::string>{"x", "y", "approx"});
}


TEST(HierBasisFunctionTest, GetSomePolynomial_3)
{
   const auto fie = GetSomePolynomial();
   const HierBasisFunction1D_HomogenousBC_Factory factory1D;
   HierBasisFunction_Factory factory(size_t{ 2 }, &factory1D);
   const HierApproximation::RefineInAllDirectionsOnL1Norm refineOnNorm{ 4 };
   auto approximation = HierApproximation::Create(*fie, factory, refineOnNorm);

   std::vector<HierMultiIndex> indices;
   str::transform(approximation->getAllTreeNodesRO(), std::back_inserter(indices), [](const HierTreeNode* hn) {return hn->BasisFunction->getMultiIndex(); });
   str::sort(indices);
   std::string str;
   for (const auto& s : indices)
   {
      str += (s.toString() + "\n");
   }
   ASSERT_EQ(approximation->getAllTreeNodesRO().size(), 17);
   ASSERT_EQ(approximation->getLeafNodesRO().size(), 12);
   ASSERT_NO_THROW(TestCollocationPoints(*fie, *approximation));


   Plotting::MdFunctionsOnUnityToFile("HierBasisFunctionTest_GetSomePolynomial_Approximation", 2, { [&approximation](std::vector<double> x) {return (*approximation)(x); } }, 20, std::vector<std::string>{"x", "y", "approx"});
}



TEST(HierBasisFunctionTest, TestConvergence)
{
   const std::array< double, 2 > xin{ 0.55, 0.85 };
   const auto fie = GetSomePolynomial();
   const double expect = fie->Evaluate(xin);

   const HierBasisFunction1D_HomogenousBC_Factory factory1D;
   HierBasisFunction_Factory factory(size_t{ 2 }, &factory1D);
   constexpr size_t maxLevel = 10;
   std::vector<double> maxSurplus;
   for (size_t level = 2; level < maxLevel; ++level)
   {
      const HierApproximation::RefineInAllDirectionsOnL1Norm refineOnNorm{ level };
      auto approximation = HierApproximation::Create(*fie, factory, refineOnNorm);
      ASSERT_NO_THROW(TestCollocationPoints(*fie, *approximation));
      const double dif = std::abs((*approximation)(xin) - expect);
   }

   for (size_t n = 2; n < maxSurplus.size(); ++n)
   {
      auto tmp = maxSurplus.at(n) / maxSurplus.at(n - 1);
      //ASSERT_TRUE(maxSurplus.at(n) < 3 * maxSurplus.at(n - 1));
   }
}

#if false
TEST(HierBasisFunctionTest, MaxSurplus)
{
   const auto fie = GetSomePolynomial();
   const HierBasisFunction1D_HomogenousBC_Factory factory1D;
   HierBasisFunction_Factory factory(size_t{ 2 }, &factory1D);
   constexpr size_t maxLevel = 10;
   std::vector<double> maxSurplus;
   for (size_t level = 2; level < maxLevel; ++level)
   {
      const HierApproximation::RefineInAllDirectionsOnL1Norm refineOnNorm{ level };
      auto approximation = HierApproximation::Create(*fie, factory, refineOnNorm);
      maxSurplus.push_back(approximation->getMaxSurplus());
   }

   for (size_t n = 2; n < maxSurplus.size(); ++n)
   {
      auto tmp = maxSurplus.at(n) / maxSurplus.at(n - 1);
      ASSERT_TRUE(maxSurplus.at(n) < 3 * maxSurplus.at(n - 1));
   }
}
#endif


