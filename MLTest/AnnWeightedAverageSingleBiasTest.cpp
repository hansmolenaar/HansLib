#include <gtest/gtest.h>
#include <array>

#include "AnnWeightedAverageSingleBias.h" 

TEST(AnnWeightedAverageSingleBiasTest, Two2One)
{
   const ML::AnnWeightedAverageSingleBias wasb(2, 1);
   const std::array< double, 3 > params = { 0.5, -0.5, 0.2 };
   const std::array<double, 2> dataIn1 = { 1, 2 };
   const std::array<double, 2> dataIn2 = { 2, 3 };
   std::array<double, 1> y;
   const std::array<double, 2> dataIn3 = { 3, 7 };

   ASSERT_EQ(wasb.getNumberOfNeuronsPrv(), 2);
   ASSERT_EQ(wasb.getNumberOfNeuronsCur(), 1);
   ASSERT_EQ(wasb.getNumberOfParameters(), 3);

   wasb.transform(dataIn1, params, y);
   ASSERT_DOUBLE_EQ(y[0], -0.3);

   wasb.transform(dataIn2, params, y);
   ASSERT_DOUBLE_EQ(y[0], -0.3);

   wasb.transform(dataIn3, params, y);
   ASSERT_DOUBLE_EQ(y[0], -1.8);
}
