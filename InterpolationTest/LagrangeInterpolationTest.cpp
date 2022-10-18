#include <gtest/gtest.h>

#include "Interpolation/LagrangeInterpolation.h"


TEST(LagrangeInterpolationTest, Basics)
{
   LagrangeInterpolation li(std::vector<double>{0, 1, 3}, std::vector<double>{0, 1, 9});
   ASSERT_FALSE(li.HasDerivative());
   ASSERT_NEAR(li.Evaluate(2.0), 4.0, 1.0e-10);
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








