#include <gtest/gtest.h>

#include "LagrangeInterpolation.h"
#include "EquidistantPoints.h"
#include "SingleVariableFunctionExamples.h"
#include "Plotting.h"
#include "ISingleVariableRealValuedFunctionUtils.h"
#include <ChebyshevPoints.h>

TEST(LagrangeInterpolationTest, Basics)
{
   LagrangeInterpolation li(std::vector<double>{0, 1, 3}, std::vector<double>{0, 1, 9});
   ASSERT_FALSE(li.HasDerivative());
   ASSERT_NEAR(li.Evaluate(2.0), 4.0, 1.0e-10);
   ASSERT_NEAR(li.Evaluate(1.0), 1.0, 1.0e-10);
}


TEST(LagrangeInterpolationTest, Example)
{
   LagrangeInterpolation li(std::vector<double>{2, 3, 5, 8, 12}, std::vector<double>{10, 15, 25, 40, 60});
   ASSERT_FALSE(li.HasDerivative());
   ASSERT_NEAR(li.Evaluate(4.0), 20.0, 1.0e-10);
}


TEST(LagrangeInterpolationTest, AnotherExample)
{
   LagrangeInterpolation li(std::vector<double>{1951, 1961, 1971}, std::vector<double>{2.8, 3.2, 4.5});
   ASSERT_FALSE(li.HasDerivative());
   ASSERT_NEAR(li.Evaluate(1966), 29.9 / 8, 1.0e-10);
}


TEST(LagrangeInterpolationTest, Runge5)
{
   std::vector<double> xvals = EquidistantPoints::Generate(-1.0, 1.0, 21);
   std::vector<double> yvals;
   const auto rungeFie = SingleVariableFunctionExamples::GetRungeFunction();
   for (double x : xvals)
   {
      yvals.push_back(rungeFie->Evaluate(x));
   }
   const LagrangeInterpolation li(xvals, yvals);
   //const auto& plot = Plotting::PlotFunction({ [&rungeFie](double x) {return rungeFie->Evaluate(x); }, [&li](double x) {return li.Evaluate(x); } }, -1, 1, 100);
}


TEST(LagrangeInterpolationTest, SkewedHat)
{
   constexpr int numPoints = 10;
   const double xpeak = std::sqrt(0.5);
   const auto hatFie = SingleVariableFunctionExamples::GetSkewedtHatFunction(xpeak);

   std::vector<double> xvals = EquidistantPoints::Generate(0.0, 1.0, numPoints-1);
   std::vector<double> yvals(numPoints);
   std::transform(xvals.begin(), xvals.end(), yvals.begin(), [&hatFie](double x) {return hatFie->Evaluate(x); });
   const LagrangeInterpolation lagrangeEquidistant(xvals, yvals);

   xvals = ChebyshevPoints::Generate(0.0, 1.0, 10);
   std::transform(xvals.begin(), xvals.end(), yvals.begin(), [&hatFie](double x) {return hatFie->Evaluate(x); });
   const LagrangeInterpolation lagrangeChebyshev(xvals, yvals);

   const auto values = Plotting::EvaluateFunctions(
      {
         [&hatFie](double x) {return hatFie->Evaluate(x); },
         [&lagrangeEquidistant](double x) {return lagrangeEquidistant.Evaluate(x); },
         [&lagrangeChebyshev](double x) {return lagrangeChebyshev.Evaluate(x); }
      },
      0.0, 1.0, 100);
   Plotting::ToFile(std::vector<std::string>{"x", "Exact", "Lagrange-Equidistant", "Lagrange-Chebyshev"}, values, "SkewedHatFunction");
}


TEST(LagrangeInterpolationTest, Step)
{
   constexpr int numPoints = 10;
   const double xpeak = std::sqrt(0.5);
   const auto hatFie = SingleVariableFunctionExamples::GetStepFunction(xpeak);

   std::vector<double> xvals = EquidistantPoints::Generate(0.0, 1.0, numPoints - 1);
   std::vector<double> yvals(numPoints);
   std::transform(xvals.begin(), xvals.end(), yvals.begin(), [&hatFie](double x) {return hatFie->Evaluate(x); });
   const LagrangeInterpolation lagrangeEquidistant(xvals, yvals);

   xvals = ChebyshevPoints::Generate(0.0, 1.0, numPoints);
   std::transform(xvals.begin(), xvals.end(), yvals.begin(), [&hatFie](double x) {return hatFie->Evaluate(x); });
   const LagrangeInterpolation lagrangeChebyshev(xvals, yvals);

   const auto values = Plotting::EvaluateFunctions(
      {
         [&hatFie](double x) {return hatFie->Evaluate(x); },
         [&lagrangeEquidistant](double x) {return lagrangeEquidistant.Evaluate(x); },
         [&lagrangeChebyshev](double x) {return lagrangeChebyshev.Evaluate(x); }
      },
      0.0, 1.0, 200);
   Plotting::ToFile(std::vector<std::string>{"x", "Exact", "Lagrange-Equidistant", "Lagrange-Chebyshev"}, values, "Stepfunction");
}







