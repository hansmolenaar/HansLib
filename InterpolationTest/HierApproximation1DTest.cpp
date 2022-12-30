#include <gtest/gtest.h>

#include "Interpolation/HierApproximation1D.h"
#include "Interpolation/HierBasisFunction1D_HomogenousBC.h"
#include "Utilities/MyException.h"
#include "Functions/SingleVariableRealValuedFunction.h"
#include "Utilities/Single.h"
#include "Functions/ISingleVariableRealValuedFunctionUtils.h"
#include "Functions/SingleVariablePolynomial.h"
#include "Interpolation/HierBasisFunction1D_ExtendedLevelOneBC.h"

inline constexpr double Epsilon = 1.0e-10;

namespace
{
   void TestCollocationPoints(const ISingleVariableRealValuedFunction& expect, const HierApproximation1D& approximation)
   {
      for (const auto& point : approximation.getCollocationPoints())
      {
         const double x = Utilities::Single(point);
         ASSERT_NEAR(approximation(x), expect.Evaluate(x), Epsilon);
      }
   }
}
TEST(HierApproximation1DTest, Basis_1_1)
{
   const HierBasisFunction1D_HomogenousBC_Factory factory;
   auto predicate = [](const HierRefinementInfo&) {return false; };
   const HierBasisFunction1D_HomogenousBC functionToApproximate(HierLevelIndex(1, 1));
   const auto approximation = HierApproximation1D::Create(functionToApproximate, factory, predicate);
   ASSERT_EQ(approximation->getCollocationPoints().size(), 1);
   ASSERT_NEAR((*approximation)(0.25), 0.5, Epsilon);
   ASSERT_NEAR((*approximation)(0.5), 1.0, Epsilon);
   ASSERT_NEAR((*approximation)(0.75), 0.5, Epsilon);
}


TEST(HierApproximation1DTest, CubicPolynomialHomogeneousBC_level2)
{
   const HierBasisFunction1D_HomogenousBC_Factory factory;
   auto predicate = [](const HierRefinementInfo& hri) {return hri.Level < 2; };
   auto functionToApproximate = SingleVariableRealValuedFunction([](double x) {return x * x * (1 - x); });
   const auto approximation = HierApproximation1D::Create(functionToApproximate, factory, predicate);
   ASSERT_EQ(approximation->getCollocationPoints().size(), 3);
   TestCollocationPoints(functionToApproximate, *approximation);

   //ISingleVariableRealValuedFunctionUtils::ToFile(functionToApproximate, *approximation, 0.0, 1.0, 1000, "CubicPolynomialHomogeneousBC_level2");
}

TEST(HierApproximation1DTest, CubicPolynomialHomogeneousBC_level5)
{
   const HierBasisFunction1D_HomogenousBC_Factory factory;
   auto predicate = [](const HierRefinementInfo& hri) {return hri.Level < 5; };
   auto functionToApproximate = SingleVariableRealValuedFunction([](double x) {return x * x * (1 - x); });
   const auto approximation = HierApproximation1D::Create(functionToApproximate, factory, predicate);
   ASSERT_EQ(approximation->getCollocationPoints().size(), 31);
   TestCollocationPoints(functionToApproximate, *approximation);

   const ISingleVariableRealValuedFunction& tmp1 = functionToApproximate;
   const ISingleVariableRealValuedFunction& tmp2 = *approximation;

   //ISingleVariableRealValuedFunctionUtils::ToFile(functionToApproximate, *approximation, 0.0, 1.0, 1000, "CubicPolynomialHomogeneousBC_level5");
}

TEST(HierApproximation1DTest, Basis_Extended)
{
   SingleVariablePolynomial functionToApproximate;
   functionToApproximate.Add(2.0, SingleVariableMonomial(0));
   functionToApproximate.Add(3.0, SingleVariableMonomial(1));

   const HierBasisFunction1D_ExtendedLevelOneBC_Factory factory;
   auto predicate = [](const HierRefinementInfo& hri) {return false; };

   const auto approximation = HierApproximation1D::Create(functionToApproximate, factory, predicate);
   ASSERT_EQ(approximation->getCollocationPoints().size(), 3);
   TestCollocationPoints(functionToApproximate, *approximation);
}

TEST(HierApproximation1DTest, Square)
{
   const SingleVariableMonomial square(2);
   constexpr size_t maxLevel = 8;
   std::vector<double> maxSurplus;
   for (size_t n = 1; n < maxLevel; ++n)
   {
      const HierBasisFunction1D_ExtendedLevelOneBC_Factory factory;
      auto predicate = [n](const HierRefinementInfo& hri) {return hri.Level < n; };

      const auto approximation = HierApproximation1D::Create(square, factory, predicate);
      TestCollocationPoints(square, *approximation);
      maxSurplus.push_back(approximation->getMaxSurplus());
   }

   for (size_t n = 1; n < maxSurplus.size(); ++n)
   {
      ASSERT_TRUE(maxSurplus.at(n) < 0.5 * maxSurplus.at(n - 1));
   }
}