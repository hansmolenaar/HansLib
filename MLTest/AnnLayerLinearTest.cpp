#include <gtest/gtest.h>
#include <array>

#include "AnnLayerLinear.h" 

TEST(AnnLayerLinearTest, Trivial)
{
   const ML::AnnLayerLinear layer(1);
   std::array<double, 1> valIn = { 2.0 };
   std::array<double, 1> valOut;

   ASSERT_EQ(layer.getNumberOfNeurons(), 1);

   layer.applyActivatorFunction(valIn, valOut);
   ASSERT_DOUBLE_EQ(valOut[0], 2.0);

   layer.applyActivatorFunctionDeriv(valIn, valOut);
   ASSERT_DOUBLE_EQ(valOut[0], 1.0);
}

TEST(AnnLayerLinearTest, Two)
{
   const ML::AnnLayerLinear layer(2);
   std::array<double, 2> valIn = { 2.0, -3.0 };
   std::array<double, 2> valOut;

   ASSERT_EQ(layer.getNumberOfNeurons(), 2);

   layer.applyActivatorFunction(valIn, valOut);
   ASSERT_DOUBLE_EQ(valOut[0], 2.0);
   ASSERT_DOUBLE_EQ(valOut[1], -3.0);

   layer.applyActivatorFunctionDeriv(valIn, valOut);
   ASSERT_DOUBLE_EQ(valOut[0], 1.0);
   ASSERT_DOUBLE_EQ(valOut[1], 1.0);
}
