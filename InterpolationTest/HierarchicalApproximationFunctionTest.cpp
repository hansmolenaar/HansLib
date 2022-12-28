#include <gtest/gtest.h>

#include "Interpolation/HierarchicalApproximationFunction.h"
#include "Utilities/MyException.h"

inline constexpr double Epsilon = 1.0e-10;

TEST(HierarchicalApproximationFunctionTest, Basis_0_1)
{
   const  HierarchicalApproximationFunction fie(HierarchicalMultiIndex({ 0, 1 }), 2.0);
   ASSERT_NEAR(fie(std::vector<double>{0.75}), 1.0, Epsilon);
}
