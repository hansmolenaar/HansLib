#include <gtest/gtest.h>

#include "Factorial.h"

TEST(FactorialTest, Basics)
{
    ASSERT_EQ(Factorial::Get(0), 1);
    ASSERT_EQ(Factorial::Get(1), 1);
    ASSERT_EQ(Factorial::Get(2), 2);
    ASSERT_EQ(Factorial::Get(3), 6);
    ASSERT_EQ(Factorial::Get(4), 24);
    ASSERT_EQ(Factorial::Get(5), 120);
}

TEST(FactorialTest, Negative)
{
    ASSERT_ANY_THROW(Factorial::Get(-1));
}