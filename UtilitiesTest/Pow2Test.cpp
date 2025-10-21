#include <gtest/gtest.h>

#include "Pow2.h"

TEST(Pow2Test, Basics)
{
    ASSERT_EQ(Pow2()(0), 1);
    ASSERT_EQ(Pow2()(1), 2);
    ASSERT_EQ(Pow2()(2), 4);
    ASSERT_EQ(Pow2()(3), 8);
    ASSERT_EQ(Pow2()(4), 16);
}