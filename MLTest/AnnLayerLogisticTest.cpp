#include <array>
#include <cmath>
#include <gtest/gtest.h>

#include "AnnLayerLogistic.h"

TEST(AnnLayerLogisticTest, Two)
{
    const ML::AnnLayerLogistic layer(2);
    std::array<double, 2> valIn = {0.0, std::log(2.0)};
    std::array<double, 2> valOut;

    ASSERT_EQ(layer.getNumberOfNeurons(), 2);

    layer.setActivatorFunction(valIn, valOut);
    ASSERT_DOUBLE_EQ(valOut[0], 0.5);
    ASSERT_DOUBLE_EQ(valOut[1], 2.0 / 3.0);

    layer.setActivatorFunctionDeriv(valIn, valOut);
    ASSERT_DOUBLE_EQ(valOut[0], 0.25);
    ASSERT_DOUBLE_EQ(valOut[1], 2.0 / 9.0);
}
