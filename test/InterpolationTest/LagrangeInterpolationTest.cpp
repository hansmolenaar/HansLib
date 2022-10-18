#include <gtest/gtest.h>

#include "LagrangeInterpolation.h"


TEST(LagrangeInterpolationTest, Basics)
{
   LagrangeInterpolation li;
   ASSERT_FALSE(li.HasDerivative());
}








