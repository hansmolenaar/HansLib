#include <gtest/gtest.h>

#include "Interpolation/LagrangeInterpolation.h"


TEST(LagrangeInterpolationTest, Basics)
{
   LagrangeInterpolation li;
   ASSERT_FALSE(li.HasDerivative());
}








