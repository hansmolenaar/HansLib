#include <gtest/gtest.h>

#include "NodalBasis1D.h"
#include "TartanGrid.h"


TEST(NodalBasis1DTest, Case1)
{
   constexpr double eps = 1.0e-10;
   auto grid = std::make_unique<TartanGrid<double, 1>>(std::vector < std::vector<double>>{ std::vector<double>{1, 2}});
   const NodalBasis1D nb1(std::move(grid), std::vector<double>{1, 9});
   ASSERT_FALSE(nb1.HasDerivative());
   ASSERT_TRUE(nb1.IsNonConstant());
   ASSERT_NEAR(nb1.Evaluate(1.0), 1.0, eps);
   ASSERT_NEAR(nb1.Evaluate(1.25), 3.0, eps);
   ASSERT_NEAR(nb1.Evaluate(1.5), 5.0, eps);
   ASSERT_NEAR(nb1.Evaluate(1.75), 7.0, eps);
   ASSERT_NEAR(nb1.Evaluate(2.0), 9.0, eps);
}

