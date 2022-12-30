#include <gtest/gtest.h>

#include "Interpolation/HierApproximation1D.h"
#include "Interpolation/HierBasisFunction1D_HomogenousBC.h"
#include "Utilities/MyException.h"
#include "Functions/SingleVariableRealValuedFunction.h"
#include "Utilities/Single.h"

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
   const auto approxomation = HierApproximation1D::Create(functionToApproximate, factory, predicate);
   ASSERT_NEAR((*approxomation)(0.25), 0.5, Epsilon);
   ASSERT_NEAR((*approxomation)(0.5), 1.0, Epsilon);
   ASSERT_NEAR((*approxomation)(0.75), 0.5, Epsilon);
}


TEST(HierApproximation1DTest, PolynomialHomogeneousBC_level2)
{
   const HierBasisFunction1D_HomogenousBC_Factory factory;
   auto predicate = [](const HierRefinementInfo&) {return false; };
   auto functionToApproximate = SingleVariableRealValuedFunction([](double x) {return x * x * (1- x); });
   const auto approxomation = HierApproximation1D::Create(functionToApproximate, factory, predicate);
   ASSERT_EQ(approxomation->getCollocationPoints().size(), 3);
   TestCollocationPoints(functionToApproximate, *approxomation);
}