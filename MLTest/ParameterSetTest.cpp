#include <gtest/gtest.h>

#include "ParameterSet.h"

TEST(ParameterSetTest, Basix)
{
    ML::ParameterSet paramSet;
    paramSet.add({1, 2});
    paramSet.add(std::vector<double>{3});

    const auto params0 = paramSet.at(0);
    ASSERT_EQ(params0.size(), 2);
    ASSERT_DOUBLE_EQ(params0[0], 1);
    ASSERT_DOUBLE_EQ(params0[1], 2);

    const auto params1 = paramSet.at(1);
    ASSERT_EQ(params1.size(), 1);
    ASSERT_DOUBLE_EQ(params1[0], 3);
}

TEST(ParameterSetTest, Errors)
{
    const ML::ParameterSet paramSet;
    ASSERT_THROW(paramSet.at(0), std::out_of_range);
}